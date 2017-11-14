#pragma once

namespace CarConfigParams
{
  /* Do not edit by hand!
     The values below is extracted from the file ##FILENAME##
     on ##DATE## with the tool ##TOOL##
   */


  /* Car config parameter number ##PARAMNUMBER## and defined values
      Parameter Description:
       ##PARAMDESC##
   */
  
  {% for param in paramlist %}
  enum class CC{{param.number}}_{{param.name}}Type
  {
      ParamNumber = {{param.number}},
      {% for value in param.values -%}
      
      {{value.desc}} = 0x{{value.value}},
      {% endfor %}
      InvalidValue = 0xFF
  };
  {% endfor %}
}

