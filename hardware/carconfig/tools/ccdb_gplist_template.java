import java.util.HashMap;
import java.util.Map;

public class generateddata
{

{% for param in paramlist %}
public enum CC_{{param.number}}_{{param.name}}
{
    {% for value in param.values -%}
    {{value.desc}}(0x{{value.value}},"{{value.desc}}")
    {%- if not loop.last -%}
    ,
    {% else %};
    {%- endif %}

    {%- endfor %}

    public final int value;
    public final String name;
    public static final int ParamNumber = {{param.number}};

    CC_{{param.number}}_{{param.name}}(int value, String name){
        this.value=value;
        this.name=name;
    }
};
{% endfor %}

}
