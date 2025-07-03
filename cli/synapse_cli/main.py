import click

# დავაიმპორტოთ ჩვენი ბრძანებების ჯგუფები
from .commands import module, project, device, config

@click.group()
def cli():
    """
    Synapse Framework CLI - a unified tool for managing your IoT projects.
    """
    pass

# დავამატოთ ბრძანებების ჯგუფები მთავარ აპლიკაციას
cli.add_command(module.module_group)
cli.add_command(project.project_group)
cli.add_command(device.device_group)
cli.add_command(config.config_group)

if __name__ == '__main__':
    cli()