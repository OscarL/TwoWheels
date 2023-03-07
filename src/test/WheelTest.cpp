// Test App for the TwoWheels input_server filter add-on.
//
// Ultra crappy code follows. BeWare!
// (what do you expect of a silly test app from a silly programmer wannabe?)
// The filter looks/is better.
//

#include <stdio.h>
#include <memory.h>

#include <Application.h>
#include <InterfaceDefs.h>
#include <StringView.h>
#include <Window.h>

const char* AppSig = "application/x-vnd.OscarL-WheelTest";

static bool success = false;

typedef struct wheel_data {
	float x_up;
	float y_up;
	float x_down;
	float y_down;
};

#define	w1_up	1
#define	w1_down	2
#define	w2_up	3
#define	w2_down 4

#define	test_done 5

class TWindow : public BWindow
{
private:
	BView* view;
	BStringView* WheelH;
	BStringView* WheelY;

	BStringView* Label1;
	BStringView* Label2;

	BStringView* ResultsH;
	BStringView* ResultsY;


	wheel_data wheel_1;
	wheel_data wheel_2;
	int test;
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

		rect = BRect(10, 15, 300, 25);
		WheelH = new BStringView(rect, "x", "Please move the first wheel up");
		view->AddChild(WheelH);

		rect = BRect(10, 30, 300, 45);
		WheelY = new BStringView(rect, "y", "(away from you)");
		view->AddChild(WheelY);


		rect = BRect(10, 60, 300, 75);
		Label1 = new BStringView(rect, "label_1", "First Wheel data:");
		view->AddChild(Label1);

		rect = BRect(10, 80, 300, 95);
		ResultsH = new BStringView(rect, "rx", "");
		view->AddChild(ResultsH);

		rect = BRect(10, 110, 300, 125);
		Label2 = new BStringView(rect, "ry", "Second Wheel data:");
		view->AddChild(Label2);

		rect = BRect(10, 130, 300, 145);
		ResultsY = new BStringView(rect, "ry", "");
		view->AddChild(ResultsY);


		test = w1_up;

		memset(&wheel_1, 0, sizeof(wheel_data));
		memset(&wheel_2, 0, sizeof(wheel_data));

		Show();
	}

	virtual void MessageReceived(BMessage *message)
	{
		switch (message->what)
		{
			case B_MOUSE_WHEEL_CHANGED:
			{
				float x = 0;
				float y = 0;

				if (B_OK == message->FindFloat("be:wheel_delta_x", &x))
				{
					switch (test)
					{
						case w1_up:		wheel_1.x_up = x;	break;
						case w1_down:	wheel_1.x_down = x;	break;
						case w2_up:		wheel_2.x_up = x;	break;
						case w2_down:	wheel_2.x_down = x;	break;
					}
				}

				if (B_OK == message->FindFloat("be:wheel_delta_y", &y))
				{
					switch (test)
					{
						case w1_up:		wheel_1.y_up = y;	break;
						case w1_down:	wheel_1.y_down = y;	break;
						case w2_up:		wheel_2.y_up = y;	break;
						case w2_down:	wheel_2.y_down = y;	break;
					}
				}

				test++;

				switch (test)
				{
					case w1_down:
						WheelH->SetText("Now move the first wheel down");
						WheelY->SetText("(towards you)");
					break;

					case w2_up:
						WheelH->SetText("This time please move the second wheel up");
						WheelY->SetText("(away from you)");
					break;
					case w2_down:
						WheelH->SetText("Last one... move the second wheel down");
						WheelY->SetText("(towards you)");
					break;
				}

				if (test == test_done)
				{
					if (
						((wheel_1.x_up != wheel_2.x_up) && (wheel_1.x_down != wheel_2.x_down) &&
						(wheel_1.x_up != wheel_1.x_down) && (wheel_2.x_up != wheel_2.x_down))
						||
						((wheel_1.y_up != wheel_2.y_up) && (wheel_1.y_down != wheel_2.y_down) &&
						(wheel_1.y_up != wheel_1.y_down) && (wheel_2.y_up != wheel_2.y_down))
					   )
					{
						WheelH->SetText("The test has ended. You may be able to use this filter");
						WheelY->SetText("with these values. Please close this window, thanks.");

						// Let's output the data in settings's file friendly format.				
						printf("# Wheel # 1 generates:\n");
						printf("x1_up = %g\nx1_down = %g\ny1_up = %g\ny1_down = %g\n", wheel_1.x_up, wheel_1.x_down, wheel_1.y_up, wheel_1.y_down);
						printf("\n# Wheel # 2 generates:\n");
						printf("x2_up = %g\nx2_down = %g\ny2_up = %g\ny2_down = %g\n", wheel_2.x_up, wheel_2.x_down, wheel_2.y_up, wheel_2.y_down);

						success = true;
					}
					else
					{
						WheelH->SetText("The test has ended. Sadly, this filter won't");
						WheelY->SetText("work with your mouse, sorry.");
					}
				}

				char buffer[128];

				sprintf(buffer, "x1_up: %g    x1_down: %g     y1_up: %g    y1_down: %g", wheel_1.x_up, wheel_1.x_down, wheel_1.y_up, wheel_1.y_down);
				ResultsH->SetText(buffer);
				sprintf(buffer, "x2_up: %g    x2_down: %g     y2_up: %g    y2_down: %g", wheel_2.x_up, wheel_2.x_down, wheel_2.y_up, wheel_2.y_down);
				ResultsY->SetText(buffer);
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

int main()
{
	BApplication app(AppSig);
	new TWindow();
	app.Run();

	if (success) return B_OK;
	else return B_ERROR;
}
