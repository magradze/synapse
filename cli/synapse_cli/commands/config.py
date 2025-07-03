import click
import json
import os
from glob import glob
from rich.console import Console
from rich.table import Table
from rich.progress import Progress

console = Console()

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
            console.print(f"Warning: Could not parse {path}: {e}", fg='yellow')
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
        console.print("[bold red]Error:[/bold red] Could not find project root. Run this from within a Synapse project.")
        return

    config_path = os.path.join(project_root, file)
    if not os.path.exists(config_path):
        console.print(f"[bold red]Error:[/bold red] Config file not found at [yellow]{config_path}[/yellow]")
        return

    console.print(f"🔎 Validating [cyan]'{config_path}'[/cyan]...")

    with Progress() as progress:
        task1 = progress.add_task("[green]Loading schemas...", total=1)
        module_schemas = load_all_module_schemas(project_root)
        progress.update(task1, advance=1)

        if not module_schemas:
            console.print("[yellow]Warning:[/yellow] No module schemas found. Cannot perform detailed validation.")
            return
        
        console.print(f"Found [bold green]{len(module_schemas)}[/bold green] module schemas to validate against.")

        task2 = progress.add_task("[green]Parsing config...", total=1)
        try:
            with open(config_path, 'r') as f:
                system_config = json.load(f)
        except (json.JSONDecodeError, IOError) as e:
            console.print(f"[bold red]Error:[/bold red] Failed to parse {config_path}: {e}")
            return
        progress.update(task2, advance=1)
    
    # ცხრილის შექმნა შედეგებისთვის
    table = Table(title="Configuration Validation Results", show_lines=True)
    table.add_column("Module Instance", style="cyan", no_wrap=True)
    table.add_column("Module Type", style="magenta")
    table.add_column("Status", justify="center")
    table.add_column("Details", style="yellow")

    errors = 0
    if 'modules' not in system_config or not isinstance(system_config['modules'], list):
        console.print("[bold red]Fatal Error:[/bold red] 'modules' key is missing or is not an array in config file.")
        return

    for i, module_config in enumerate(system_config['modules']):
        module_type = module_config.get('type')
        instance_name = module_config.get('config', {}).get('instance_name', f"Unnamed Module #{i+1}")

        if not module_type:
            table.add_row(instance_name, "[dim]N/A[/dim]", "❌ [red]FAIL[/red]", "Field 'type' is missing.")
            errors += 1
            continue

        if module_type not in module_schemas:
            table.add_row(instance_name, f"'{module_type}'", "❌ [red]FAIL[/red]", "Unknown module type.")
            errors += 1
            continue
        
        schema = module_schemas[module_type]
        missing_params = []
        if 'required_config' in schema:
            for required_param in schema['required_config']:
                if required_param not in module_config.get('config', {}):
                    missing_params.append(f"'{required_param}'")
        
        if missing_params:
            errors += len(missing_params)
            details = f"Missing required config parameter(s): {', '.join(missing_params)}"
            table.add_row(instance_name, module_type, "❌ [red]FAIL[/red]", details)
        else:
            table.add_row(instance_name, module_type, "✅ [green]PASS[/green]", "[dim]All required parameters are present.[/dim]")

    console.print(table)
    
    if errors == 0:
        console.print("\n[bold green]✅ Validation successful! No errors found.[/bold green]")
    else:
        console.print(f"\n[bold red]❌ Validation failed with {errors} error(s).[/bold red]")