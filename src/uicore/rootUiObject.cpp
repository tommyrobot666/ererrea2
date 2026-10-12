#include <uicore/rootUiObject.h>

void rootUiObject::renderAll() {
    glm::mat4& orthProj = OrthProjManager::getOrthProjForAnchor(anchor);
    renderThenChildren(orthProj);
}

void rootUiObject::updateAll() {
    startUiUpdate();
    uiUpdate();
}
