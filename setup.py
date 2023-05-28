from pathlib import Path

import setuptools

try:
    from cxbuild.cmake_extension import CMakeExtension
    from cxbuild.extension_builder import ExtensionBuilder

    setuptools.setup(
        ext_modules=[
            CMakeExtension(
                name="attacus.attacus",
                source_dir=Path.cwd(),
                install_from=Path('attacus'),
            ),
        ],
        cmdclass=dict(
            build_ext=ExtensionBuilder,
        ),
    )
except ImportError:
    setuptools.setup()