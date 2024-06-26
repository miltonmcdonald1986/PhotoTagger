#include <filesystem>
#include <format>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

class Application
{
public:
	Application()
	{
		if (!al_init())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize Allegro5.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_init_font_addon())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize font addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_init_native_dialog_addon())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize native dialog addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_init_ttf_addon())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize ttf addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		m_pFontCour = al_load_font(R"(C:\Windows\Fonts\cour.ttf)", 24, 0);
		if (!m_pFontCour)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to load Courier New Regular font.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_init_image_addon())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to initialize image addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		m_pDisplay = al_create_display(800, 600);
		if (!m_pDisplay)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to create display.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		m_pEventQueue = al_create_event_queue();
		if (!m_pEventQueue)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to create event queue.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		ALLEGRO_EVENT_SOURCE* pDisplayEventSource = al_get_display_event_source(m_pDisplay);
		if (!pDisplayEventSource)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to get display event source", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_install_mouse())
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to install mouse.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		ALLEGRO_EVENT_SOURCE* pMouseEventSource = al_get_mouse_event_source();
		if (!pMouseEventSource)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to get mouse event source", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		al_register_event_source(m_pEventQueue, pDisplayEventSource);
		al_register_event_source(m_pEventQueue, pMouseEventSource);

		m_IsInitialized = true;
	}

	~Application()
	{
		al_destroy_event_queue(m_pEventQueue);
		al_destroy_display(m_pDisplay);
		al_destroy_font(m_pFontCour);
	}

	bool IsInitialized()
	{
		return m_IsInitialized;
	}

	void Run()
	{
		ALLEGRO_FILECHOOSER* pFileChooser = al_create_native_file_dialog(R"(%USERPROFILE%)", "Select Directory", "*.*", ALLEGRO_FILECHOOSER_FOLDER);
		if (!pFileChooser)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to create native file dialog.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if (!al_show_native_file_dialog(m_pDisplay, pFileChooser))
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to show native file dialog.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		auto dirPath = al_get_native_file_dialog_path(pFileChooser, 0);
		for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath))
		{
			if (entry.is_regular_file())
			{
				al_set_window_title(m_pDisplay, entry.path().string().c_str());
			}
		}

		al_destroy_native_file_dialog(pFileChooser);

		ALLEGRO_BITMAP* pBitmap = al_load_bitmap(R"(C:\Users\milto\OneDrive\Documents\Pictures\Our Organized Photos\2024\01-2024\20240105_183951.jpg)");
		if (!pBitmap)
		{
			al_show_native_message_box(nullptr, "Error", "Allegro5", "Failed to load bitmap.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		bool done = false;
		while (!done)
		{
			ALLEGRO_EVENT alEvent;
			al_get_next_event(m_pEventQueue, &alEvent);
			switch (alEvent.type)
			{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			default:
				break;
			}

			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_scaled_bitmap(pBitmap, 0., 0., al_get_bitmap_width(pBitmap), al_get_bitmap_height(pBitmap), 0., 0., 800, 600, 0);

			ALLEGRO_MOUSE_STATE mouseState;
			al_get_mouse_state(&mouseState);
			int mouseX = al_get_mouse_state_axis(&mouseState, 0);
			int mouseY = al_get_mouse_state_axis(&mouseState, 1);
			al_draw_text(m_pFontCour, al_map_rgb(255, 255, 255), 0, 0, ALLEGRO_ALIGN_LEFT, std::format("MousePos: {}, {}", mouseX, mouseY).c_str());

			


			al_flip_display();
		}
	}

private:
	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;
	Application(Application&& other) = delete;
	Application& operator=(Application&& other) = delete;

	ALLEGRO_BITMAP* m_pBitmap = nullptr;
	ALLEGRO_DISPLAY* m_pDisplay = nullptr;
	ALLEGRO_EVENT_QUEUE* m_pEventQueue = nullptr;
	ALLEGRO_FONT* m_pFontCour = nullptr;
	bool m_IsInitialized = false;
};

int main(int argc, const char** argv)
{
	Application app;
	if (!app.IsInitialized())
		return 1;

	app.Run();

	return 0;
}
