import setuptools

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()


def get_version():
    version = {}
    with open("src/dx_engine/version.py") as fp:
        exec(fp.read(), version)
    return version["__version__"]


setuptools.setup(
    name="dx-engine",
    version=get_version(),
    author="JG Jang",
    author_email="jgjang@deepx.co.kr",
    description="Python wrapper class for dx_rt",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://gh.deepx.ai/deepx/dx_rt.git",
    project_urls={
        "Bug Tracker": "https://gh.deepx.ai/deepx/dx_rt/issues",
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    packages=setuptools.find_packages(where="src"),
    package_dir={"": "src"},
    package_data={
        'dx_engine': ['capi/*.so', 'capi/*.lib', 'capi/*.dll', 'capi/*.pyd'],
    },
    entry_points={
        'console_scripts': [
            'dxrt_bitmatch = dx_engine.bitmatch.cli:main',
        ],
    },
    python_requires=">=3.6",
    install_requires=["numpy"],
)
