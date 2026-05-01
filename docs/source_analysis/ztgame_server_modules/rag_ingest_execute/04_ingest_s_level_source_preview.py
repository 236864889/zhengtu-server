#!/usr/bin/env python3
import json
import re
import hashlib
from pathlib import Path
from datetime import datetime

ROOT = Path(__file__).resolve().parents[4]
S_LIST_MD = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/04_s_level_source_ingest_list.md"
MANIFEST = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/01_global_rag_ingest_manifest.dedup.jsonl"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/s_level_source_chunks.preview.jsonl"


def read_manifest():
    rows = []
    if not MANIFEST.exists():
        return rows
    for line in MANIFEST.read_text(encoding="utf-8").splitlines():
        if line.strip():
            rows.append(json.loads(line))
    return rows


def discover_sources():
    picked = []
    if S_LIST_MD.exists():
        for line in S_LIST_MD.read_text(encoding="utf-8").splitlines():
            m = re.search(r"(ztgame/.+)", line)
            if m:
                p = ROOT / m.group(1).strip()
                if p.exists() and p.is_file():
                    picked.append(p)
    if picked:
        return sorted(set(picked))

    for row in read_manifest():
        path = row.get("path", "")
        level = str(row.get("level", "")).upper()
        if level == "S" and path.startswith("ztgame/"):
            p = ROOT / path
            if p.exists() and p.is_file():
                picked.append(p)
    return sorted(set(picked))


def chunk_source(path: Path, max_lines=80):
    lines = path.read_text(encoding="utf-8", errors="ignore").splitlines()
    chunks = []
    start = 1
    while start <= len(lines):
        end = min(start + max_lines - 1, len(lines))
        body = "\n".join(lines[start - 1:end]).strip()
        if body:
            chunks.append((start, end, body))
        start = end + 1
    return chunks


def main():
    dry_run = True
    files = discover_sources()
    OUT.parent.mkdir(parents=True, exist_ok=True)
    now = datetime.utcnow().isoformat() + "Z"
    count = 0

    with OUT.open("w", encoding="utf-8") as f:
        for path in files:
            rel = path.relative_to(ROOT).as_posix()
            for idx, (line_start, line_end, content) in enumerate(chunk_source(path), start=1):
                chunk_id = hashlib.md5(f"{rel}:{idx}:{line_start}:{line_end}".encode("utf-8")).hexdigest()
                row = {
                    "chunk_id": chunk_id,
                    "path": rel,
                    "level": "S",
                    "line_start": line_start,
                    "line_end": line_end,
                    "content": content,
                    "source_type": "source",
                    "collection": "ztgame_server_core",
                    "created_at": now,
                }
                f.write(json.dumps(row, ensure_ascii=False) + "\n")
                count += 1

    print(f"[INFO] dry_run={dry_run}")
    print(f"[INFO] source files={len(files)}")
    print(f"[INFO] preview out={OUT}")
    print(f"[INFO] chunk count={count}")
    print("[INFO] no qdrant call executed.")


if __name__ == "__main__":
    main()
