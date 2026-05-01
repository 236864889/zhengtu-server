#!/usr/bin/env python3
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
LIST_MD = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/03_markdown_docs_ingest_list.md"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/markdown_chunks.preview.jsonl"


def parse_list():
    mapping = {}
    pat = re.compile(r"\|\s*`([^`]+\.md)`\s*\|\s*`([^`]+)`\s*\|\s*`([^`]+)`\s*\|")
    for line in LIST_MD.read_text(encoding="utf-8", errors="replace").splitlines():
        m = pat.search(line)
        if m:
            mapping[m.group(1)] = {
                "recommended_collection": m.group(2),
                "recommended_level": m.group(3),
            }
    return mapping

def chunks(text, size=800):
    text = text.strip()
    for i in range(0, len(text), size):
        yield text[i:i+size]

def main():
    mapping = parse_list()
    rows = 0
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with OUT.open("w", encoding="utf-8") as fw:
        for path, meta in mapping.items():
            fp = ROOT / path
            if not fp.exists():
                continue
            txt = fp.read_text(encoding="utf-8", errors="replace")
            for idx, ck in enumerate(chunks(txt), 1):
                row = {
                    "path": path,
                    "chunk_id": f"{path}#md#{idx}",
                    "recommended_collection": meta.get("recommended_collection", "ztgame_server_core"),
                    "recommended_level": meta.get("recommended_level", "B"),
                    "content_preview": ck,
                }
                fw.write(json.dumps(row, ensure_ascii=False) + "\n")
                rows += 1
    print(f"ROOT={ROOT}")
    print(f"output={OUT}")
    print(f"rows={rows}")

if __name__ == '__main__':
    main()
