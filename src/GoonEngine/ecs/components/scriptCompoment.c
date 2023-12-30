#include <GoonEngine/gnpch.h>
#include <GoonEngine/ecs/components/scriptComponent.h>

ScriptComponent *gnScriptComponentNew()
{
    ScriptComponent *scriptComponent = calloc(1, sizeof(*scriptComponent));
    return scriptComponent;
}
int gnScriptComponentSetUpdateFunc(ScriptComponent* scriptComponent, ScriptComponentDelegate func)
{
    scriptComponent->UpdateFunc = func;
}