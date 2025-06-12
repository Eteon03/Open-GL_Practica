#include "../Headers/SceneGraphNode.hpp"

namespace udit
{
    SceneGraphNode::SceneGraphNode()
        : local_transform(1.0f)
    {
    }

    void SceneGraphNode::set_transform(const glm::mat4& transform)
    {
        local_transform = transform;
    }

    void SceneGraphNode::set_draw_callback(DrawCallback callback)
    {
        draw_callback = callback;
    }

    void SceneGraphNode::add_child(Ptr child)
    {
        children.push_back(child);
    }

    void SceneGraphNode::draw(const glm::mat4& parent_transform) const
    {
        glm::mat4 global_transform = parent_transform * local_transform;

        if (draw_callback)
        {
            draw_callback(global_transform);
        }

        for (const auto& child : children)
        {
            child->draw(global_transform);
        }
    }
    glm::mat4 SceneGraphNode::get_transform() const
    {
        return local_transform;
    }
}