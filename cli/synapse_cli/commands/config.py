import click
import json
import os
from glob import glob

# --- Helper Functions ---

def find_project_root():
    """Finds the project root by looking for the 'main' directory."""
    current_dir = os.getcwd()
    while current_dir != '/':
        if os.path.isdir(os.path.join(current_dir, 'main')):
            return current_dir
        current_dir = os.path.dirname(current_dir)
    return None

def load_all_module_schemas(project_root):
    """Scans all module.json files and loads them."""
    schemas = {}
    module_paths = glob(os.path.join(project_root, 'components', 'modules', '*', '*', 'module.json'))
    for path in module_paths:
        try:
            with open(path, 'r') as f:
                schema = json.load(f)
                if 'name' in schema:
                    schemas[schema['name']] = schema
        except (json.JSONDecodeError, IOError) as e:
            click.secho(f"Warning: Could not parse {path}: {e}", fg='yellow')
    return schemas

# --- Command Group ---

@click.group(name="config")
def config_group():
    """Commands for managing system_config.json."""
    pass

# --- Commands ---

@config_group.command(name="validate")
@click.option("--file", "-f", default="configs/system_config.json", help="Path to the config file to validate.")
def validate_command(file):
    """
    Validates the system_config.json file against module schemas.
    """
    project_root = find_project_root()
    if not project_root:
        click.secho("Error: Could not find project root. Run this command from within a Synapse project.", fg="red")
        return

    config_path = os.path.join(project_root, file)
    if not os.path.exists(config_path):
        click.secho(f"Error: Config file not found at {config_path}", fg="red")
        return

    click.echo(f"Validating '{config_path}'...")

    # 1. Load all available module schemas
    module_schemas = load_all_module_schemas(project_root)
    if not module_schemas:
        click.secho("Warning: No module schemas found. Cannot perform detailed validation.", fg='yellow')
        return
    
    click.echo(f"Found {len(module_schemas)} module schemas to validate against.")

    # 2. Load the system config
    try:
        with open(config_path, 'r') as f:
            system_config = json.load(f)
    except (json.JSONDecodeError, IOError) as e:
        click.secho(f"Error: Failed to parse {config_path}: {e}", fg="red")
        return

    # 3. Perform validation
    errors = 0
    warnings = 0

    if 'modules' not in system_config or not isinstance(system_config['modules'], list):
        click.secho("Error: 'modules' key is missing or is not an array.", fg="red")
        return

    for i, module_config in enumerate(system_config['modules']):
        module_type = module_config.get('type')
        instance_name = module_config.get('config', {}).get('instance_name', f"module_{i}")

        # Check if module type exists
        if not module_type:
            click.secho(f"  [ERROR] Module '{instance_name}': 'type' field is missing.", fg='red')
            errors += 1
            continue

        if module_type not in module_schemas:
            click.secho(f"  [ERROR] Module '{instance_name}': Unknown module type '{module_type}'.", fg='red')
            errors += 1
            continue
        
        # Check for required config parameters (if defined in module.json)
        schema = module_schemas[module_type]
        if 'required_config' in schema:
            for required_param in schema['required_config']:
                if required_param not in module_config.get('config', {}):
                    click.secho(f"  [ERROR] Module '{instance_name}' (type: {module_type}): Missing required config parameter '{required_param}'.", fg='red')
                    errors += 1

    click.echo("-" * 30)
    if errors == 0:
        click.secho("Validation successful! No errors found.", fg='green')
    else:
        click.secho(f"Validation failed with {errors} error(s).", fg='red')