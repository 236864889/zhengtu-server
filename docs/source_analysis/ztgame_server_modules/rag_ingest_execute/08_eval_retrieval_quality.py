#!/usr/bin/env python3
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
CASES = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/07_rag_qa_eval_cases.md"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/retrieval_eval_table.jsonl"


def parse_cases(text):
    blocks = re.split(r"(?m)^##\s+", text)
    rows = []
    for block in blocks:
        b = block.strip()
        if not b:
            continue
        lines = b.splitlines()
        title = lines[0].strip()

        def pick(prefix):
            for line in lines[1:]:
                if line.startswith(prefix):
                    return line[len(prefix):].strip()
            return ""

        row = {
            "case_title": title,
            "question": pick("- 问题："),
            "expect_collection": pick("- 期望召回 collection："),
            "expect_module": pick("- 期望召回模块："),
            "expect_keywords": pick("- 期望关键词："),
            "expect_doc": pick("- 期望引用文档："),
            "acceptance": pick("- 验收标准："),
            "retrieved_docs": [],
            "retrieval_score": None,
            "review_note": "",
        }
        if row["question"]:
            rows.append(row)
    return rows


def retrieval_placeholder(_query):
    return []


def main():
    dry_run = True
    if not CASES.exists():
        print(f"[ERROR] cases file not found: {CASES}")
        return 1

    rows = parse_cases(CASES.read_text(encoding="utf-8"))
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with OUT.open("w", encoding="utf-8") as f:
        for row in rows:
            if not dry_run:
                row["retrieved_docs"] = retrieval_placeholder(row["question"])
            f.write(json.dumps(row, ensure_ascii=False) + "\n")

    print(f"[INFO] dry_run={dry_run}")
    print(f"[INFO] parsed cases={len(rows)}")
    print(f"[INFO] output={OUT}")
    print("[INFO] retrieval interface is placeholder only.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
