import click
import subprocess
import serial
import time
import os

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

# --- Commands ---
@device_group.command(name="shell")
@click.option("--port", "-p", default=None, help="Serial port for the shell.")
def shell_command(port):
    """Opens an interactive shell to the device."""
    port = port or find_serial_port()
    if not port:
        click.secho("Could not find device serial port. Please specify with -p.", fg="red")
        return
        
    click.echo(f"Starting interactive shell on {port}... (Press Ctrl+] to exit)")
    
    # We use idf.py monitor for the interactive shell experience
    idf_path = os.getenv("IDF_PATH")
    if not idf_path:
        click.secho("IDF_PATH environment variable is not set.", fg="red")
        return
        
    command = f"source {idf_path}/export.sh && idf.py -p {port} monitor"
    try:
        subprocess.run(command, shell=True, check=True, executable="/bin/bash")
    except subprocess.CalledProcessError:
        click.secho("Monitor command failed.", fg="red")


@device_group.command(name="cmd")
@click.argument("command_string", nargs=-1)
@click.option("--port", "-p", default=None, help="Serial port to send the command.")
def cmd_command(command_string, port):
    """Sends a single command to the device and prints the output."""
    port = port or find_serial_port()
    if not port:
        click.secho("Could not find device serial port. Please specify with -p.", fg="red")
        return

    if not command_string:
        click.secho("Error: No command provided.", fg="red")
        return

    full_command = " ".join(command_string)
    
    try:
        with serial.Serial(port, 115200, timeout=1) as ser:
            click.echo(f"Sending to {port}: '{full_command}'")
            ser.write(f"{full_command}\r\n".encode('utf-8'))
            
            # Give the device a moment to respond
            time.sleep(0.5) 
            
            response = ser.read_all().decode('utf-8', errors='ignore')
            click.echo("--- Device Response ---")
            click.echo(response)
            click.echo("-----------------------")

    except serial.SerialException as e:
        click.secho(f"Serial error: {e}", fg="red")

@device_group.command(name="health")
@click.option("--port", "-p", default=None, help="Serial port.")
def health_command(port):
    """A shortcut for 'device cmd health'."""
    # This uses the context to call another command
    ctx = click.get_current_context()
    ctx.invoke(cmd_command, command_string=("health",), port=port)