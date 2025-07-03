import click
import os
import subprocess
from rich.console import Console
from rich.panel import Panel
from rich.progress import track

console = Console()

@click.group(name="module")
def module_group():
    """Commands for managing framework modules."""
    pass

@module_group.command(name="create")
@click.argument("module_name")
@click.option("--type", "-t", required=True, help="Module type (e.g., system, sensors).")
@click.option("--desc", "-d", required=True, help="A short description for the module.")
def module_create(module_name, type, desc):
    """
    Creates a new module skeleton using create_module.py.
    """
    console.print(f"🛠️ Creating new module: [bold cyan]{module_name}[/bold cyan]")

    # ვიპოვოთ create_module.py სკრიპტის გზა
    script_path = os.path.join(os.getcwd(), "scripts", "create_module.py")
    if not os.path.exists(script_path):
        console.print("❌ [bold red]Error:[/bold red] create_module.py not found in 'scripts' directory!")
        return

    command = [
        "python3",
        script_path,
        module_name,
        "--type", type,
        "--description", desc
    ]

    # დააყენე progress bar
    for _ in track(range(1), description="🚀 [green]Running module generator...[/green]"):
        try:
            subprocess.run(command, check=True)
        except subprocess.CalledProcessError as e:
            console.print(f"❌ [bold red]Failed to create module:[/bold red] {e}")
            return

    console.print(f"✅ [bold green]Module '{module_name}' created successfully![/bold green]")
    console.print("[yellow]💡 Tip:[/yellow] Don't forget to run 'idf.py reconfigure' to let the build system find it.")
