#!/usr/bin/env python3
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
EVAL_MD = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_ready/06_rag_qa_eval_template.md"
OUT = ROOT / "docs/source_analysis/ztgame_server_modules/rag_ingest_execute/out/retrieval_eval_table.jsonl"

HEADERS = [
"启动失败","登录失败","网关转发失败","进场景失败","角色保存失败","充值扣费异常","区服列表异常","Lua脚本异常","任务不推进","工具误用风险","配置漂移"
]

def main():
    text = EVAL_MD.read_text(encoding='utf-8', errors='replace').splitlines()
    found = [line.strip().lstrip('#').strip() for line in text if line.strip().startswith('## ')]
    with OUT.open('w', encoding='utf-8') as fw:
        for h in found:
            fw.write(json.dumps({"category": h, "question": "", "status": "template"}, ensure_ascii=False)+"\n")
    print(f"ROOT={ROOT}")
    print(f"output={OUT}")
    print(f"rows={len(found)}")
    print(f"expected_11={len([h for h in HEADERS if h in found])==11}")

if __name__ == '__main__':
    main()
