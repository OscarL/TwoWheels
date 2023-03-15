#include <Application.h>
#include <StringView.h>
#include <Window.h>


const char* AppSig = "application/x-vnd.OscarL-WheelTest";


class TWindow : public BWindow
{
private:
	BView* view;
	BStringView* Label1;
	BStringView* Label2;
	BStringView* Label3;

public:
	TWindow()
		: BWindow(BRect (0, 0, 350, 175), "Mouse Wheel Tester", B_FLOATING_WINDOW_LOOK,
		  B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
	{
		MoveTo(100, 100);

		BRect rect = Bounds();
		view = new BView(rect, "view", B_FOLLOW_ALL, B_WILL_DRAW);
		view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		AddChild(view);

		view->SetFontSize(10.0);

		rect = BRect(10, 15, 300, 60);
		Label1 = new BStringView(rect, "Label1", "Place the cursor over this window,\n"
			"and move your mouse's scroll wheel(s)");
		view->AddChild(Label1);

		rect = BRect(10, 75, 300, 90);
		Label2 = new BStringView(rect, "Label2", "Wheel(s) data:");
		view->AddChild(Label2);

		rect = BRect(10, 115, 300, 130);
		Label3 = new BStringView(rect, "Label3", "");
		view->AddChild(Label3);

		Show();
	}

	virtual void MessageReceived(BMessage* message)
	{
		switch (message->what)
		{
			case B_MOUSE_WHEEL_CHANGED:
			{
				float x = 0;
				float y = 0;

				if (message->FindFloat("be:wheel_delta_x", &x) != B_OK)
					x = 0;

				if (message->FindFloat("be:wheel_delta_y", &y) != B_OK)
					y = 0;

				BString buffer;
				buffer.SetToFormat("wheel_delta_x: %g    wheel_delta_y: %g", x, y);
				Label3->SetText(buffer);
			}
			break;

			default:
				BWindow::MessageReceived(message);
			break;
		}
	}

	bool QuitRequested()
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
		return true;
	}
};


int
main()
{
	BApplication app(AppSig);
	new TWindow();
	app.Run();

	return B_OK;
}
