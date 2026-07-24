#include <stdio.h>
#include <limits.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "external/nuklear.h"

#define NK_GLFW_GL3_IMPLEMENTATION
#include "external/nuklear_glfw_gl3.h"

#include "data_structures/int_list.h"

int main()
{
    GLFWwindow *window;
    struct nk_glfw glfw = {0};
    struct nk_context *ctx;
    struct nk_font_atlas *atlas;

    IntList list;
    int input_value = 0;
    int input_index = 0;
    int get_result = 0;
    int find_result = -1;

    bool has_get_result = false;
    bool get_succeeded = false;

    bool has_find_result = false;

    bool has_contains_result = false;
    bool contains_result = false;

    init_int_list(&list);

    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1000, 700, "C Data Structures Visualizer", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        free_int_list(&list);
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        free_int_list(&list);
        return 1;
    }

    glfwSwapInterval(1);

    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);

    while (!glfwWindowShouldClose(window))
    {
        // input
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);

        // construct UI
        if (nk_begin(ctx, "Integer List", nk_rect(20, 20, 960, 660), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(ctx, 30, 3);
            nk_property_int(ctx, "Value", INT_MIN, &input_value, INT_MAX, 1, 1);

            if (nk_button_label(ctx, "Append")) {
                append_int_list(&list, input_value);
            }

            if (nk_button_label(ctx, "Insert")) {
                insert_int_list(&list, input_value, input_index);
            }

            nk_layout_row_dynamic(ctx, 30, 3);
            nk_property_int(ctx, "Index", 0, &input_index, 1000000, 1, 1);

            if (nk_button_label(ctx, "Remove index")) {
                remove_int_list_by_index(&list, input_index);
            }

            if (nk_button_label(ctx, "Remove head")) {
                remove_int_list_head(&list);
            }

            nk_layout_row_dynamic(ctx, 30, 2);

            if (nk_button_label(ctx, "Get at index")) {
                has_get_result = true;

                if (input_index >= 0 && (size_t)input_index < list.count) {
                    get_result = get_int_list(&list, input_index);
                    get_succeeded = true;
                } else {
                    get_succeeded = false;
                }
            }

            if (!has_get_result) {
                nk_label(ctx, "Get: no query yet", NK_TEXT_LEFT);
            } else if (get_succeeded)
            {
                nk_labelf(ctx, NK_TEXT_LEFT, "Get [%d] = %d", input_index, get_result);
            } else {
                nk_labelf(ctx, NK_TEXT_LEFT, "Get: index %d is out of bounds", input_index);
            }

            nk_layout_row_dynamic(ctx, 30, 2);

            if (nk_button_label(ctx, "Find value")) {
                find_result = find_int_list(&list, input_value);
                has_find_result = true;
            }

            if (!has_find_result) {
                nk_label(ctx, "Find: no query yet", NK_TEXT_LEFT);
            } else if (find_result >= 0 ) {
                nk_labelf(ctx, NK_TEXT_LEFT, "Find %d: index %d", input_value, find_result);
            } else {
                nk_labelf(ctx, NK_TEXT_LEFT, "Find %d: not found", input_value);
            }

            nk_layout_row_dynamic(ctx, 30, 2);

            if (nk_button_label(ctx, "Contains value")) {
                contains_result = contains_int_list(&list, input_value);
                has_contains_result = true;
            }

            if (!has_contains_result)
            {
                nk_label(ctx, "Contains: no query yet", NK_TEXT_LEFT);
            } else {
                nk_labelf(ctx, NK_TEXT_LEFT, "Contains %d: %s", input_value, contains_result ? "yes" : "no");
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Clear")) {
                clear_int_list(&list);
            }

            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_button_label(ctx, "Seed list 1 - 5")){
                seed_int_list(&list);
                has_get_result = false;
                has_find_result = false;
                has_contains_result = false;
            }

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_labelf(ctx, NK_TEXT_LEFT, "Count: %zu    Capcity %zu", list.count, list.capacity);

            if (list.count > 0)
            {
                int columns = list.count < 8 ? (int)list.count : 8;
                nk_layout_row_dynamic(ctx, 60, columns);

                for (size_t i = 0; i < list.count; i++)
                {
                    nk_labelf(ctx, NK_TEXT_CENTERED, "[%zu]: %d", i, list.items[i]);
                }
            }
        }
        nk_end(ctx);

        // render
        {
            int width;
            int height;

            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);

            glfwSwapBuffers(window);
        }
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    free_int_list(&list);

    return 0;
}
