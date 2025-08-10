#!/usr/bin/env python3
import sys
import json
from pathlib import Path
from jsonschema import validate, exceptions

def main():
    if len(sys.argv) != 4:
        print("Usage: python validate_jsons.py <file_to_validate> <schema_file> <file_type>")
        sys.exit(1)

    file_to_validate = Path(sys.argv[1])
    schema_file = Path(sys.argv[2])
    file_type = sys.argv[3]

    try:
        with open(file_to_validate, 'r') as f:
            instance_data = json.load(f)
        with open(schema_file, 'r') as f:
            schema_data = json.load(f)

        # config.json is an array of objects, module.json is a single object
        if file_type == "config":
            if not isinstance(instance_data, list):
                 raise exceptions.ValidationError("config.json must be an array of objects.")
            for item in instance_data:
                validate(instance=item, schema=schema_data)
        else: # module
            validate(instance=instance_data, schema=schema_data)

        print(f"✅ Validation successful for: {file_to_validate.name}")

    except FileNotFoundError as e:
        print(f"❌ Error: File not found - {e.filename}", file=sys.stderr)
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"❌ Error: Invalid JSON in {file_to_validate}: {e.msg}", file=sys.stderr)
        sys.exit(1)
    except exceptions.ValidationError as e:
        print(f"❌ Schema validation failed for {file_to_validate}:", file=sys.stderr)
        print(f"   - Error: {e.message}", file=sys.stderr)
        print(f"   - Path: {list(e.path)}", file=sys.stderr)
        print(f"   - Validator: {e.validator} = {e.validator_value}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"❌ An unexpected error occurred: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()