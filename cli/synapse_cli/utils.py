import os
import subprocess
from rich.console import Console

console = Console()

def run_idf_command(args, shell_mode=False):
    """
    A robust helper function to run idf.py commands.
    Checks for IDF_PATH and executes the command.
    """
    idf_path = os.getenv("IDF_PATH")
    if not idf_path:
        console.print("[bold red]Error:[/bold red] IDF_PATH environment variable is not set. Please source `export.sh` first.")
        return False

    # For commands like `idf.py monitor` that are interactive
    if shell_mode:
        command = f"source {idf_path}/export.sh && idf.py {' '.join(args)}"
        executable = "/bin/bash"
    else:
        # For non-interactive commands, we can be more direct
        command = [os.path.join(idf_path, 'tools', 'idf.py')] + args
        executable = None

    console.print(f"▶️  Running: [cyan]idf.py {' '.join(args)}[/cyan]")
    
    try:
        # Using a direct command list is safer than shell=True when possible
        subprocess.run(command, shell=shell_mode, check=True, executable=executable)
        return True
    except subprocess.CalledProcessError as e:
        console.print(f"❌ [bold red]Command failed with exit code {e.returncode}[/bold red]")
        return False
    except FileNotFoundError:
        console.print("❌ [bold red]Error:[/bold red] 'idf.py' not found. Is ESP-IDF installed and sourced correctly?")
        return False