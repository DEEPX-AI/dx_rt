![dxrt_r2](https://user-images.githubusercontent.com/79885630/198581277-3abc611a-a759-455f-9a81-5b4739727162.png)

# DXRT (DEEPX Runtime) by DeepX
**DXRT** is an inference framework designed for optimized execution of inference tasks using the DEEPX AI accelerator and DEEPX NPU.  
With simple APIs compatible with various applications, DXRT empowers developers to achieve high-performance inference smoothly.  

## Features
- **Hardware Acceleration**: Facilitates seamless AI acceleration through a hardware interface to DEEPX devices.  
- **Model Compatibility**: Supports pre-built models from the DEEPX model zoo and compiled models by DXCOM (DEEPX Compiler).  
- **Optimized Latency**: Achieves low-latency performance for parallel processing.  
- **Dynamic Execution Environment**: Provides flexible runtime environment adaptable to various applications.  
- **Easy Integration**: Streamlined API for straightforward integration with existing applications.  
- **Configurable Parameters**: Fine-tune inference settings through user-configurable parameters.  

## Supported devices
|Device|Type|  
|---|---|  
|DX_L1|Standalone device|  
|DX_L2|Standalone device|  
|DX_M1|AI Accelerator|  
|DX_H1|AI Accelerator|  

## Resources
### Model zoo (Prebuilt models)
Official modelzoo is in preparation.  
### Documents
Official website documents are in preparation.  
Currently, documents are provided with limited rights. Please consult with our contact point person.  
In other way, you can generate documents from repository (using markdown files in `docs`).
* python>=3.9 is needed.  
```
# install MkDocs
pip install mkdocs mkdocs-material mkdocs-video pymdown-extensions
# generate html to directory "docs_generated"
mkdocs build
```
You can also generate API reference using doxygen.
```
# install doxygen
sudo apt install doxygen graphviz
# generate API reference html
cd docs/cpp_api
doxygen Doxyfile
```

## License
This project is licensed under the [MIT License](LICENSE).  
