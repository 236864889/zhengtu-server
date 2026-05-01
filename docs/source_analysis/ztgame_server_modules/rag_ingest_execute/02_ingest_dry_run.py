#!/usr/bin/env python3
import json
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
MANIFEST = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/01_global_rag_ingest_manifest.dedup.jsonl"

def main():
    rows = []
    missing = []
    coll = Counter()
    level = Counter()
    for i, line in enumerate(MANIFEST.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
        if not line.strip():
            continue
        row = json.loads(line)
        rows.append(row)
        collection = row.get("recommended_collection") or row.get("collection") or "unknown_collection"
        coll[collection] += 1
        level[row.get("level", "unknown")] += 1
        p = ROOT / row.get("path", "")
        if not p.exists():
            missing.append((i, row.get("path", "")))
    print(f"ROOT={ROOT}")
    print(f"manifest={MANIFEST}")
    print(f"rows={len(rows)}")
    print("collection_distribution=")
    for k, v in coll.most_common():
        print(f"  - {k}: {v}")
    print("level_distribution=")
    for k, v in level.most_common():
        print(f"  - {k}: {v}")
    print(f"missing_paths={len(missing)}")
    for item in missing[:20]:
        print("  ", item)

if __name__ == '__main__':
    main()
