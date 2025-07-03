import click
import subprocess
import os

# --- Helper Function ---
def run_idf_command(args):
    """A helper function to run idf.py commands."""
    try:
        # We need to source the export.sh script to set up the environment
        # This is a bit tricky, so we run it as a shell command.
        idf_path = os.getenv("IDF_PATH")
        if not idf_path:
            click.secho("IDF_PATH environment variable is not set. Please source export.sh.", fg="red")
            return
        
        command = f"source {idf_path}/export.sh && idf.py {' '.join(args)}"
        click.echo(f"Running command: {command}")
        subprocess.run(command, shell=True, check=True, executable="/bin/bash")

    except subprocess.CalledProcessError as e:
        click.secho(f"Command failed with exit code {e.returncode}", fg="red")
    except FileNotFoundError:
        click.secho("Error: 'idf.py' not found. Is ESP-IDF installed and sourced correctly?", fg="red")


# --- Command Group ---
@click.group(name="project")
def project_group():
    """Commands for building and managing the project."""
    pass

# --- Commands ---
@project_group.command(name="build")
@click.option("--reconfigure", is_flag=True, help="Run reconfigure before building.")
def build_command(reconfigure):
    """Builds the Synapse project."""
    args = []
    if reconfigure:
        args.append("reconfigure")
    
    args.append("build")
    run_idf_command(args)

@project_group.command(name="flash")
@click.option("--port", "-p", default=None, help="Serial port to flash.")
def flash_command(port):
    """Flashes the built project to the device."""
    args = ["flash"]
    if port:
        args.extend(["-p", port])
    run_idf_command(args)

@project_group.command(name="monitor")
@click.option("--port", "-p", default=None, help="Serial port to monitor.")
def monitor_command(port):
    """Starts the serial monitor."""
    args = ["monitor"]
    if port:
        args.extend(["-p", port])
    run_idf_command(args)

@project_group.command(name="clean")
def clean_command():
    """Cleans the build directory."""
    run_idf_command(["fullclean"])