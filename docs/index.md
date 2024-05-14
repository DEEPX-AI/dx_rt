![dxrt_r2](/assets/images/introuduction.jpg)

**DX-RT** is DEEPX Runtime SDK for AI inference based on DEEPX devices.  
It supports pre-built models from DEEPX model zoo, and compiled models by DXCOM(DEEPX Compiler SDK).  
## Supported devices
DX-RT provides common inference framework based on 2 kinds of inference mode.  

* Accelerator mode : inference by PCIe interface  
* Standalone mode : inference by direct AXI/APB interface  

|Device|Mode|  
|---|---|  
|DX_V1|Standalone|  
|DX_V3|Standalone|  
|DX_M1|Accelerator|  
|DX_H1|Accelerator|  

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
