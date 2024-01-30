from mako.template import Template
from mako.lookup import TemplateLookup

def generate_template_module(input_excel, template_file, output_file, module, npu_variant):
    open(output_file, 'w').write(
        Template(filename=template_file).render(
            input_excel = input_excel, 
            module = module,
            npu_variant = npu_variant
        ).replace('\r\n', '\n')
    )
    return

def generate_template_modules(input_excel, template_file, output_file, modules, npu_modules, npu_variant):
    open(output_file, 'w').write(
        Template(filename=template_file).render(
            input_excel = input_excel, 
            modules = modules,
            npu_modules = npu_modules,
            npu_variant = npu_variant
        ).replace('\r\n', '\n')
    )
    return
