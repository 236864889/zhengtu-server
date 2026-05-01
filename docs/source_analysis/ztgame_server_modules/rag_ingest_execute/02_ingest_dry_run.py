#!/usr/bin/env python3
import json
from pathlib import Path
from collections import Counter

ROOT = Path(__file__).resolve().parents[4]
MANIFEST = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/01_global_rag_ingest_manifest.dedup.jsonl"


def load_jsonl(path: Path):
    rows = []
    with path.open("r", encoding="utf-8") as f:
        for i, line in enumerate(f, start=1):
            text = line.strip()
            if not text:
                continue
            try:
                rows.append(json.loads(text))
            except json.JSONDecodeError as exc:
                raise ValueError(f"Invalid JSONL at line {i}: {exc}")
    return rows


def main():
    dry_run = True
    print(f"[INFO] dry_run={dry_run}")
    print(f"[INFO] manifest={MANIFEST}")
    if not MANIFEST.exists():
        print("[ERROR] manifest not found, cannot continue.")
        return 1

    rows = load_jsonl(MANIFEST)
    collections = Counter()
    missing = []

    for row in rows:
        collection = row.get("collection", "unknown")
        collections[collection] += 1
        path = row.get("path")
        if path:
            file_path = ROOT / path
            if not file_path.exists():
                missing.append(path)

    print("\n[SUMMARY] collection counts")
    for name, count in sorted(collections.items()):
        print(f"  - {name}: {count}")

    print(f"\n[SUMMARY] total records: {len(rows)}")
    print(f"[SUMMARY] missing files: {len(missing)}")
    if missing:
        for p in missing[:50]:
            print(f"  - {p}")
        if len(missing) > 50:
            print("  - ... (truncated)")

    print("\n[PLAN] files that would be ingested:")
    for row in rows[:50]:
        print(f"  - {row.get('collection','unknown')} :: {row.get('path','<no path>')}")
    if len(rows) > 50:
        print("  - ... (truncated)")

    print("\n[INFO] dry-run only. no qdrant and no embedding call executed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
