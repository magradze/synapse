import click
from pyfiglet import figlet_format
from rich.console import Console
from rich.panel import Panel
from rich.text import Text
import rich_click

# Import command groups
from .commands import module, project, device, config

# Rich Console
console = Console()

# -------- rich-click პარამეტრები --------
# (თქვენი პარამეტრები უცვლელია, ისინი კარგია)
rich_click.rich_click.USE_RICH_MARKUP = True
rich_click.rich_click.STYLE_HELPTEXT = "dim"
# ... დანარჩენი პარამეტრები ...

# -------- CLI Root --------
@click.group(cls=rich_click.RichGroup, invoke_without_command=True)
@click.pass_context
def cli(ctx):
    """
    [bold green]Synapse Framework CLI[/bold green]

    A unified tool for managing your IoT projects.
    Use it to create modules, manage devices, configure the environment, and build projects.
    """
    # აჩვენე ბანერი მხოლოდ მაშინ, თუ ქვე-ბრძანება არ არის გამოძახებული
    # (ანუ, მომხმარებელმა გაუშვა "synapse" ან "synapse --help")
    if ctx.invoked_subcommand is None:
        # 1. ვქმნით ASCII არტს
        banner_text = figlet_format("Synapse CLI", font="slant")
        
        # 2. ვშლით ტექსტს ხაზებად და თითოეულს ვანიჭებთ გრადიენტს
        lines = banner_text.split("\n")
        for line in lines:
            # ვქმნით Text ობიექტს თითოეული ხაზისთვის და ვბეჭდავთ
            line_text = Text(line, style="bold deep_sky_blue1 to dark_violet")
            console.print(line_text)

        # 4. ვბეჭდავთ გაფერადებულ Text ობიექტს
        console.print(Panel.fit(
            "[bold green]Welcome to Synapse CLI[/bold green] 🚀\n"
            "[yellow]Manage your IoT modules, devices, and projects with style![/yellow]\n\n"
            "[dim]Type [italic]synapse --help[/italic] to get started.[/dim]",
            title="[cyan]Info[/cyan]",
            border_style="green"
        ))
        # 2. პროგრამულად ვაჩვენებთ დახმარების ტექსტს
        console.print("\n") # დავამატოთ ცარიელი ხაზი ვიზუალური გამოყოფისთვის
        click.echo(ctx.get_help())

# -------- Register Commands --------
cli.add_command(module.module_group)
cli.add_command(project.project_group)
cli.add_command(device.device_group)
cli.add_command(config.config_group)

# -------- Entry Point --------
if __name__ == "__main__":
    cli()