#include <sqlite_orm/sqlite_orm.h>
#include <crow.h>

#include "AppManager.h"

int main()
{
	crow::SimpleApp app;
	auto storage = createStorage("TRIV");
	storage.sync_schema();
	database::populateDatabaseWithQuestions(storage);

	AppManager appManager(app, storage);

	app.port(18080).multithreaded().run();

	return 0;
}