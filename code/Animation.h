#pragma once

//
// Sonme utility functions
//
static float AnimationGetClipDuration(cgltf_animation *animationClip)
{
    float duration = 0;
    float lastMax  = 0;

    for (size_t i = 0; i < animationClip->samplers_count; i++) {
        cgltf_accessor *input = animationClip->samplers[i].input;

        for (size_t j = 0; j < NB_OF_ELEMENTS_IN_ARRAY(input->max); j++) {
            if (input->has_max && input->max[j] > lastMax)
                lastMax = input->max[j];
        }
        duration = lastMax;
    }
    return duration;
}

static int AnimationMaxSampleCount(cgltf_animation *animationClip)
{
    float last_max = 0;

    for (size_t i = 0; i < animationClip->samplers_count; i++) {
        auto *input = animationClip->samplers[i].input;

        if (input->count > last_max) {
            last_max = input->count;
        }
    }
    return last_max;
}

static float readFloatFromAccessor(cgltf_accessor *accessor, cgltf_size idx)
{
    float value;
    // SDL_Log("idx:[%d]", idx);
    if (cgltf_accessor_read_float(accessor, idx, &value, accessor->count))
        return value;
    else
        return 0;
}

static glm::vec3 getVec3AtKeyframe(cgltf_animation_sampler *sampler, int keyframe)
{
    // todo: check for failure?
    void *values = (void *)((uint8_t *)sampler->output->buffer_view->buffer->data + sampler->output->buffer_view->offset);
    return ((glm::vec3 *)values)[keyframe];
}
static glm::quat getQuatAtKeyframe(cgltf_animation_sampler *sampler, int keyframe)
{
    // todo: check for failure?
    void *values = (void *)((uint8_t *)sampler->output->buffer_view->buffer->data + sampler->output->buffer_view->offset);
    return ((glm::quat *)values)[keyframe];
}

/////////////////////////////////////




// note: this is unsued right now
std::vector<Transform> bindPoseLocalJointTransforms;
static void            ComputeLocalJointTransforms(cgltf_data *data)
{
    cgltf_node **joints = data->skins->joints;
    bindPoseLocalJointTransforms.resize(data->skins->joints_count);

    for (size_t joint_idx = 0; joint_idx < data->skins->joints_count; joint_idx++) {
        bindPoseLocalJointTransforms[joint_idx].translation = *(glm::vec3 *)joints[joint_idx]->translation;
        glm::vec3 R                                         = glm::eulerAngles(*(glm::quat *)joints[joint_idx]->rotation);
        bindPoseLocalJointTransforms[joint_idx].rotation    = R;
        bindPoseLocalJointTransforms[joint_idx].scale       = glm::vec3(1);

        bindPoseLocalJointTransforms[joint_idx].name = joints[joint_idx]->name;

        if (joint_idx == 0) continue;

        if (joints[joint_idx]->parent == joints[joint_idx - 1]) {
            bindPoseLocalJointTransforms[joint_idx].parent       = &bindPoseLocalJointTransforms[joint_idx - 1];
            bindPoseLocalJointTransforms[joint_idx].parent->name = bindPoseLocalJointTransforms[joint_idx - 1].name;

            bindPoseLocalJointTransforms[joint_idx - 1].child       = &bindPoseLocalJointTransforms[joint_idx];
            bindPoseLocalJointTransforms[joint_idx - 1].child->name = bindPoseLocalJointTransforms[joint_idx].name;
        } else {
            // find index of parent joint
            auto currentJointParent = joints[joint_idx]->parent;
            for (size_t i = 0; i < data->skins->joints_count; i++) {
                if (joints[i] == currentJointParent) {
                    bindPoseLocalJointTransforms[joint_idx].parent       = &bindPoseLocalJointTransforms[i];
                    bindPoseLocalJointTransforms[joint_idx].parent->name = bindPoseLocalJointTransforms[i].name;
                }
            }
        }
    }


    // for (size_t joint_idx = 0; joint_idx < data->skins->joints_count; joint_idx++) {

    //     cgltf_node *parent = NULL;
    //     Transform  *Tr     = NULL;


    //     SDL_Log("\n-- Joint[%s]: --", data->skins->joints[joint_idx]->name);
    //     SDL_Log("-----------TR-------------");
    //     if ((Tr = bindPoseLocalJointTransforms[joint_idx].parent) != NULL)
    //         for (size_t i = 0; Tr;) {
    //             SDL_Log("Child[%s]:", Tr->name);
    //             Tr = Tr->parent;
    //         }
    //     SDL_Log("-----------NODE-------------");

    //     if ((parent = data->skins->joints[joint_idx]->parent) != NULL)
    //         for (size_t i = 0; parent;) {
    //             SDL_Log("Child[%s]:", parent->name);
    //             parent = parent->parent;
    //         }
    // }
}
