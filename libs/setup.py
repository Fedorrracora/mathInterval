from setuptools import setup, Extension, find_packages
setup(
    name="mathInterval",
    version="0.1",
    packages=["mathInterval"],
    package_data={"mathInterval": ["mathInterval.so", "mathInterval.pyi"]},
    include_package_data=True,

)
