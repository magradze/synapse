import click
from .. import utils

@click.group(name="project")
def project_group():
    """Commands for building and managing the project."""
    pass

@project_group.command(name="build")
@click.option("--reconfigure", is_flag=True, help="Run reconfigure before building.")
def build_command(reconfigure):
    """Builds the Synapse project."""
    args = []
    if reconfigure:
        args.append("reconfigure")
    args.append("build")
    utils.run_idf_command(args)

@project_group.command(name="flash")
@click.option("--port", "-p", default=None, help="Serial port to flash.")
def flash_command(port):
    """Flashes the built project to the device."""
    args = ["flash"]
    if port:
        args.extend(["-p", port])
    utils.run_idf_command(args)

@project_group.command(name="monitor")
@click.option("--port", "-p", default=None, help="Serial port to monitor.")
def monitor_command(port):
    """Starts the serial monitor."""
    args = ["monitor"]
    if port:
        args.extend(["-p", port])
    # Monitor is interactive, so we need shell mode
    utils.run_idf_command(args, shell_mode=True)

@project_group.command(name="clean")
def clean_command():
    """Cleans the build directory (fullclean)."""
    utils.run_idf_command(["fullclean"])