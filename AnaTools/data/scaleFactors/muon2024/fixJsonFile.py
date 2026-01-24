#!/usr/bin/env python3
import json
from pathlib import Path

inp = Path("ScaleFactors_Muon_Z_HLT_2024_eta_pt_schemaV2.json")
out = inp.with_suffix(".flowfix.json")

data = json.loads(inp.read_text())

def add_flow(node):
    if isinstance(node, dict):
        if node.get("nodetype") == "binning" and "flow" not in node:
            node["flow"] = "error"   # or "error"/"nan" if you prefer
        for v in node.values():
            add_flow(v)
    elif isinstance(node, list):
        for x in node:
            add_flow(x)

add_flow(data)
out.write_text(json.dumps(data, indent=2, sort_keys=False))
print(f"Wrote {out}")
