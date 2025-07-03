from setuptools import setup, find_packages

setup(
    name="synapse-cli",
    version="1.0.0",
    packages=find_packages(),
    include_package_data=True,
    install_requires=[
        'click',
        'pyyaml',
        'rich',
        'pyfiglet',
        'rich_click',
    ],
    entry_points={
        'console_scripts': [
            'synapse = synapse_cli.main:cli',
        ],
    },
)