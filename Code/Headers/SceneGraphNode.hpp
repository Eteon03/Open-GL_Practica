#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace udit
{
    class SceneGraphNode
    {
    public:
        using Ptr = std::shared_ptr<SceneGraphNode>;
        using DrawCallback = std::function<void(const glm::mat4&)>;

    private:
        glm::mat4 local_transform;
        DrawCallback draw_callback;
        std::vector<Ptr> children;

    public:
        SceneGraphNode();
        void set_transform(const glm::mat4& transform);
        void set_draw_callback(DrawCallback callback);
        void add_child(Ptr child);
        void draw(const glm::mat4& parent_transform = glm::mat4(1.0f)) const;
        glm::mat4 get_transform()const;
    };
}