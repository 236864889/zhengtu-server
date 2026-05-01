#!/usr/bin/env python3
import json
import re
import hashlib
from pathlib import Path
from datetime import datetime

ROOT = Path(__file__).resolve().parents[4]
TARGET = ROOT / "docs/source_analysis/ztgame_server_modules"
LIST_MD = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/03_markdown_docs_ingest_list.md"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/markdown_chunks.preview.jsonl"


def discover_markdown_files():
    files = []
    if LIST_MD.exists():
        for line in LIST_MD.read_text(encoding="utf-8").splitlines():
            m = re.search(r"(docs/source_analysis/ztgame_server_modules/.+?\.md)", line)
            if m:
                p = ROOT / m.group(1)
                if p.exists():
                    files.append(p)
    if not files:
        files = sorted(TARGET.rglob("*.md"))
    return files


def split_markdown(path: Path):
    text = path.read_text(encoding="utf-8", errors="ignore")
    parts = re.split(r"(?m)^(#{1,6}\s+.+)$", text)
    chunks = []
    if len(parts) <= 1:
        chunks.append(("FULL", text.strip()))
        return chunks
    current_title = "PREFACE"
    for part in parts:
        if not part.strip():
            continue
        if re.match(r"^#{1,6}\s+", part):
            current_title = part.strip()
            continue
        body = part.strip()
        if body:
            chunks.append((current_title, body))
    return chunks


def qdrant_upsert_placeholder(_points):
    return "placeholder_not_executed"


def embedding_placeholder(_texts):
    return "placeholder_not_executed"


def main():
    dry_run = True
    files = discover_markdown_files()
    OUT.parent.mkdir(parents=True, exist_ok=True)
    now = datetime.utcnow().isoformat() + "Z"

    count = 0
    with OUT.open("w", encoding="utf-8") as f:
        for path in files:
            rel = path.relative_to(ROOT).as_posix()
            for idx, (title, content) in enumerate(split_markdown(path), start=1):
                if not content:
                    continue
                chunk_id = hashlib.md5(f"{rel}:{idx}:{title}".encode("utf-8")).hexdigest()
                row = {
                    "chunk_id": chunk_id,
                    "path": rel,
                    "title": title,
                    "content": content,
                    "source_type": "markdown",
                    "collection": "ztgame_server_core",
                    "created_at": now,
                }
                f.write(json.dumps(row, ensure_ascii=False) + "\n")
                count += 1

    print(f"[INFO] dry_run={dry_run}")
    print(f"[INFO] markdown files={len(files)}")
    print(f"[INFO] chunk preview written: {OUT}")
    print(f"[INFO] chunk count={count}")
    if not dry_run:
        embedding_placeholder([])
        qdrant_upsert_placeholder([])


if __name__ == "__main__":
    main()
