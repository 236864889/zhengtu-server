#!/usr/bin/env python3
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
S_MD = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/04_s_level_source_ingest_list.md"
MANIFEST = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/01_global_rag_ingest_manifest.dedup.jsonl"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/s_level_source_chunks.preview.jsonl"


def parse_manifest():
    m = {}
    for line in MANIFEST.read_text(encoding='utf-8', errors='replace').splitlines():
        if not line.strip():
            continue
        row = json.loads(line)
        m[row.get("path","")] = row
    return m

def parse_s_paths():
    paths = []
    for line in S_MD.read_text(encoding='utf-8', errors='replace').splitlines():
        m = re.search(r"`(ztgame/[^`]+)`", line)
        if m:
            paths.append(m.group(1))
    return paths

def main():
    manifest = parse_manifest()
    paths = parse_s_paths()
    count = 0
    missing = 0
    with OUT.open('w', encoding='utf-8') as fw:
        for p in paths:
            fp = ROOT / p
            if not fp.exists():
                missing += 1
                continue
            txt = fp.read_text(encoding='utf-8', errors='replace')[:1200]
            meta = manifest.get(p, {})
            row = {
                "path": p,
                "module": meta.get("module", "unknown"),
                "domain": meta.get("domain", "unknown"),
                "recommended_collection": meta.get("recommended_collection", "ztgame_server_core"),
                "keywords": meta.get("keywords", []),
                "reason": meta.get("reason", ""),
                "content_preview": txt,
            }
            fw.write(json.dumps(row, ensure_ascii=False) + "\n")
            count += 1
    print(f"ROOT={ROOT}")
    print(f"output={OUT}")
    print(f"rows={count}")
    print(f"missing_paths={missing}")

if __name__ == '__main__':
    main()
