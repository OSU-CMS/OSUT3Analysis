import argparse
import gzip
import json

DESCRIPTION = ""

def getargs():
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("jec_config")
    parser.add_argument("year")

    data_mc_group = parser.add_mutually_exclusive_group(required=True)
    data_mc_group.add_argument("--data", action="store_true")
    data_mc_group.add_argument("--mc", action="store_true")

    parser.add_argument("--output", default="trimmed_json.json.gz")
    return parser.parse_args()

def load_json(jec_config, year):
    with open(jec_config) as f:
        config = json.load(f)

    correction_path = config[year]["jercJsonPath"]

    with gzip.open(correction_path, mode="rt", encoding="utf-8") as f:
        full_json = json.load(f)

    return full_json

def get_corr_by_name(corrections, name_parts):
    return [c for c in corrections if all(part in c["name"] for part in name_parts)]

def trim_json(full_json, is_data):
    trimmed_json = {
        "schema_version": full_json["schema_version"],
        "corrections": [],
        "compound_corrections": []
    }

    if is_data:
        corrections = get_corr_by_name(full_json["corrections"], ["DATA"])

        # JecConfigReader always reads in these two for data and MC
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "PtResolution"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "ScaleFactor"])[0])
    else:
        corrections = []
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "L1FastJet"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "L2Relative"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "L3Absolute"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "L2L3Residual"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "Total_AK4PFPuppi"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "PtResolution"])[0])
        corrections.append(get_corr_by_name(full_json["corrections"], ["MC", "ScaleFactor"])[0])

    trimmed_json["corrections"] = corrections
    return trimmed_json

def save_json(trimmed_json, output):
    with gzip.open(output, "wt", encoding="utf-8") as f:
        json.dump(trimmed_json, f)

def main():
    args = getargs()

    full_json = load_json(args.jec_config, args.year)
    trimmed_json = trim_json(full_json, args.data)
    save_json(trimmed_json, args.output)

if __name__ == "__main__":
    main()
