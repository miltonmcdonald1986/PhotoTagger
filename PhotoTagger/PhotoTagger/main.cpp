#include <format>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

int main(int argc, const char** argv)
{
	if (!al_init())
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize Allegro5.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	if (!al_init_font_addon())
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize font addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	if (!al_init_ttf_addon())
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize ttf addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	ALLEGRO_FONT *pFontCour = al_load_font(R"(C:\Windows\Fonts\cour.ttf)", 24, 0);
	if (!pFontCour)
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to load Courier New Regular font.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	if (!al_init_image_addon())
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize image addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	ALLEGRO_DISPLAY* pDisplay = al_create_display(800, 600);
	if (!pDisplay)
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to create display.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	ALLEGRO_EVENT_QUEUE* pEventQueue = al_create_event_queue();
	if (!pEventQueue)
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to create event queue.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	ALLEGRO_EVENT_SOURCE* pDisplayEventSource = al_get_display_event_source(pDisplay);
	if (!pDisplayEventSource)
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to get display event source", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	if (!al_install_mouse())
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to install mouse.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	ALLEGRO_EVENT_SOURCE* pMouseEventSource = al_get_mouse_event_source();
	if (!pMouseEventSource)
	{
		al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to get mouse event source", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return 1;
	}

	al_register_event_source(pEventQueue, pDisplayEventSource);
	al_register_event_source(pEventQueue, pMouseEventSource);

	bool done = false;
	while (!done)
	{
		ALLEGRO_EVENT alEvent;
		al_get_next_event(pEventQueue, &alEvent);
		switch (alEvent.type)
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		default:
			break;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		
		ALLEGRO_MOUSE_STATE mouseState;
		al_get_mouse_state(&mouseState);
		int mouseX = al_get_mouse_state_axis(&mouseState, 0);
		int mouseY = al_get_mouse_state_axis(&mouseState, 1);
		al_draw_text(pFontCour, al_map_rgb(255, 255, 255), 0, 0, ALLEGRO_ALIGN_LEFT, std::format("MousePos: {}, {}", mouseX, mouseY).c_str());

		al_flip_display();
	}

	al_destroy_font(pFontCour);
	al_destroy_event_queue(pEventQueue);
	al_destroy_display(pDisplay);

	return 0;
}
