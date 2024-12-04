#include "libs.h"
#include "PowerSupply.h"

class PowerManager
{
public:
  void run()
  {
    while (running)
    {
      system("cls");
      PowerSupply powerSupply;
      powerSupply.update();
      std::cout << powerSupply;

      displayMenu();
      Sleep(1000); // 1 sec
    }
  }

  void stop()
  {
    running = false;
  }

private:
  bool running = true;
  void displayMenu() const
  {
    std::cout << "Options:" << std::endl
              << "Press 0 to Exit" << std::endl
              << "Press 1 to Sleep" << std::endl
              << "Press 2 to Hibernate" << std::endl;
  }
};

int main()
{
  PowerManager powerManager;
  std::thread autoupdate(&PowerManager::run, &powerManager);

  char key = '1';
  while (key != '0')
  {
    key = _getch();

    switch (key)
    {
    case '0':
      powerManager.stop();
      break;

    case '1':
      SetSuspendState(FALSE, FALSE, FALSE);
      break;

    case '2':
      SetSuspendState(TRUE, FALSE, FALSE);
      break;
    }
  }

  autoupdate.join();
  return 0;
}