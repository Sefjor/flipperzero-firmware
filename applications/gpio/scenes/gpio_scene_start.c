#include "../gpio_app_i.h"
#include "furi-hal-power.h"

enum GpioItem {
    GpioItemOtg,
    GpioItemTest,
    GpioItemUsbUart,
};

enum GpioOtg {
    GpioOtgOff,
    GpioOtgOn,
    GpioOtgSettingsNum,
};

const char* const gpio_otg_text[GpioOtgSettingsNum] = {
    "Off",
    "On",
};

static void gpio_scene_start_var_list_enter_callback(void* context, uint32_t index) {
    furi_assert(context);
    GpioApp* app = context;
    if(index == GpioItemTest) {
        view_dispatcher_send_custom_event(
            app->view_dispatcher, GPIO_SCENE_START_CUSTOM_EVENT_TEST);
    } else if(index == GpioItemUsbUart) {
        view_dispatcher_send_custom_event(
            app->view_dispatcher, GPIO_SCENE_START_CUSTOM_EVENT_USB_UART);
    }
}

static void gpio_scene_start_var_list_change_callback(VariableItem* item) {
    GpioApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);

    variable_item_set_current_value_text(item, gpio_otg_text[index]);
    if(index == GpioOtgOff) {
        view_dispatcher_send_custom_event(
            app->view_dispatcher, GPIO_SCENE_START_CUSTOM_EVENT_OTG_OFF);
    } else if(index == GpioOtgOn) {
        view_dispatcher_send_custom_event(
            app->view_dispatcher, GPIO_SCENE_START_CUSTOM_EVENT_OTG_ON);
    }
}

void gpio_scene_start_on_enter(void* context) {
    GpioApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;

    VariableItem* item;
    variable_item_list_set_enter_callback(
        var_item_list, gpio_scene_start_var_list_enter_callback, app);
    item = variable_item_list_add(
        var_item_list,
        "5V on GPIO",
        GpioOtgSettingsNum,
        gpio_scene_start_var_list_change_callback,
        app);
    if(furi_hal_power_is_otg_enabled()) {
        variable_item_set_current_value_index(item, GpioOtgOn);
        variable_item_set_current_value_text(item, gpio_otg_text[GpioOtgOn]);
    } else {
        variable_item_set_current_value_index(item, GpioOtgOff);
        variable_item_set_current_value_text(item, gpio_otg_text[GpioOtgOff]);
    }
    variable_item_list_add(var_item_list, "GPIO tester", 0, NULL, NULL);
    variable_item_list_add(var_item_list, "USB-UART bridge", 0, NULL, NULL);

    variable_item_list_set_selected_item(
        var_item_list, scene_manager_get_scene_state(app->scene_manager, GpioSceneStart));

    view_dispatcher_switch_to_view(app->view_dispatcher, GpioAppViewVarItemList);
}

bool gpio_scene_start_on_event(void* context, SceneManagerEvent event) {
    GpioApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == GPIO_SCENE_START_CUSTOM_EVENT_OTG_ON) {
            furi_hal_power_enable_otg();
        } else if(event.event == GPIO_SCENE_START_CUSTOM_EVENT_OTG_OFF) {
            furi_hal_power_disable_otg();
        } else if(event.event == GPIO_SCENE_START_CUSTOM_EVENT_TEST) {
            scene_manager_set_scene_state(app->scene_manager, GpioSceneStart, 1);
            scene_manager_next_scene(app->scene_manager, GpioSceneTest);
        } else if(event.event == GPIO_SCENE_START_CUSTOM_EVENT_USB_UART) {
            scene_manager_set_scene_state(app->scene_manager, GpioSceneStart, 2);
            scene_manager_next_scene(app->scene_manager, GpioSceneUsbUart);
        }
        consumed = true;
    }
    return consumed;
}

void gpio_scene_start_on_exit(void* context) {
    GpioApp* app = context;
    variable_item_list_clean(app->var_item_list);
}
