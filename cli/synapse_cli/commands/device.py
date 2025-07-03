import click
import serial
import time
import os
from rich.console import Console
from rich.panel import Panel
from .. import utils

console = Console()

# --- Helper Function ---
def find_serial_port():
    """Tries to find a common ESP32 serial port."""
    common_ports = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/tty.SLAB_USBtoUART', '/dev/tty.usbserial-0001']
    for port in common_ports:
        if os.path.exists(port):
            return port
    return None

# --- Command Group ---
@click.group(name="device")
def device_group():
    """Commands for interacting with a connected device."""
    pass

@device_group.command(name="shell")
@click.option("--port", "-p", default=None, help="Serial port for the shell.")
def shell_command(port):
    """Opens an interactive shell to the device."""
    port_to_use = port or find_serial_port()
    if not port_to_use:
        console.print("[bold red]Error:[/bold red] Could not find device serial port. Please specify with -p.")
        return
        
    console.print(f"🔌 Starting interactive shell on [bold cyan]{port_to_use}[/bold cyan]... (Press Ctrl+] to exit)")
    
    # ვიყენებთ ცენტრალიზებულ ფუნქციას
    utils.run_idf_command(["-p", port_to_use, "monitor"], shell_mode=True)


@device_group.command(name="cmd")
@click.argument("command_string", nargs=-1)
@click.option("--port", "-p", default=None, help="Serial port to send the command.")
def cmd_command(command_string, port):
    """Sends a single command to the device and prints the output."""
    port_to_use = port or find_serial_port()
    if not port_to_use:
        console.print("[bold red]Error:[/bold red] Could not find device serial port. Please specify with -p.")
        return

    if not command_string:
        console.print("[bold red]Error:[/bold red] No command provided.")
        return

    full_command = " ".join(command_string)
    
    try:
        with serial.Serial(port_to_use, 115200, timeout=1) as ser:
            console.print(f"➡️  Sending to [cyan]{port_to_use}[/cyan]: '[yellow]{full_command}[/yellow]'")
            ser.write(f"{full_command}\r\n".encode('utf-8'))
            
            time.sleep(0.5) 
            
            response = ser.read_all().decode('utf-8', errors='ignore').strip()
            
            console.print(Panel(
                response if response else "[dim]No response from device.[/dim]",
                title="[green]Device Response[/green]",
                border_style="blue",
                expand=False
            ))

    except serial.SerialException as e:
        console.print(f"❌ [bold red]Serial error:[/bold red] {e}")

@device_group.command(name="health")
@click.option("--port", "-p", default=None, help="Serial port.")
def health_command(port):
    """A shortcut for 'device cmd health'."""
    # This uses the context to call another command
    ctx = click.get_current_context()
    ctx.invoke(cmd_command, command_string=("health",), port=port)