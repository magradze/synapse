import click
import os
import subprocess

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
    click.echo(f"Creating new module: {module_name}")
    
    # ვიპოვოთ create_module.py სკრიპტის გზა
    script_path = os.path.join(os.getcwd(), "scripts", "create_module.py")
    if not os.path.exists(script_path):
        click.secho("Error: create_module.py not found in 'scripts' directory!", fg="red")
        return

    # გამოვიძახოთ ჩვენი არსებული Python სკრიპტი
    command = [
        "python3",
        script_path,
        module_name,
        "--type", type,
        "--description", desc
    ]
    
    try:
        subprocess.run(command, check=True)
        click.secho(f"Module '{module_name}' created successfully!", fg="green")
        click.echo("Don't forget to run 'idf.py reconfigure' to let the build system find it.")
    except subprocess.CalledProcessError as e:
        click.secho(f"Failed to create module. Error: {e}", fg="red")