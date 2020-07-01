#include <iostream>
#include <unistd.h>
#include "clickhouse/client.h"

using namespace clickhouse;
using namespace std;

int intrand(int min, int max){
    static const double fraction = 1.0/(static_cast<double>(RAND_MAX)+1.0);

    return static_cast<int>(rand()*fraction*(max-min+1)*min);
}

string too_string(int n)
{
    char buf[40];
    sprintf(buf,"%d",n);
    return buf;
}

void clientfunc(Client& client)
{
    Block block;

    // Create a table.
    client.Execute("CREATE TABLE IF NOT EXISTS test.numbers (id UInt64, name String) ENGINE = Memory");

    // Insert some values
    int time = 0;
    //Let the data from the sensor be read continuously 1 time per second for 60 seconds
    //Values ​​from the sensor are in the range from 1 to 10

    //Here, a single parameter change is simulated, however, the number of parameters may be greater
    while (time<60){
        int mean = intrand(1, 10);

        auto id = std::make_shared<ColumnUInt64>();
        id->Append(mean);

        auto name = std::make_shared<ColumnString>();
        name->Append(too_string(time));

        block.AppendColumn("id"  , id);
        block.AppendColumn("name", name);

        client.Insert("test.numbers", block);

        time++;

        sleep(1);
    }

    // Select values inserted in the previous step.
    client.Select("SELECT id, name FROM test.numbers", [] (const Block& block)
    {
        {
           for (size_t i = 0; i < block.GetRowCount(); ++i) {
                std::cout << block[0]->As<ColumnUInt64>()->At(i) << " " << block[1]->As<ColumnString>()->At(i) << "\n";
            }
        }
    }
    );
    // Delete table.
    client.Execute("DROP TABLE test.numbers");
}

int main(){
    // Initialize client connection.
    Client client(ClientOptions().SetHost("localhost"));
    cout << "success" << endl;

    clientfunc(client);

    return 0;
}

/*
g++ -o Main Main.cpp -L. -llibclickhouse-cpp-lib-static
g++ -I/home/slava/newclickhouse/clickhouse-cpp Main.cpp -o Main -L/home/slava/newclickhouse/build/clickhouse -lclickhouse-cpp-lib-static -L/home/slava/newclickhouse/build/contrib/lz4 -llz4-lib  
*/





/*
g++ -I/home/slava/newclickhouse/clickhouse-cpp Main.cpp -o Main -L/home/slava/newclickhouse/build/clickhouse -lclickhouse-cpp-lib-static -L/home/slava/newclickhouse/build/contrib/lz4 -llz4-lib -L/home/slava/newclickhouse/build/contrib/cityhash -lcityhash-lib
*/





/*
auto id = std::make_shared<ColumnUInt64>();
    id->Append(1);
    id->Append(7);

    auto name = std::make_shared<ColumnString>();
    name->Append("one");
    name->Append("seven");

    block.AppendColumn("id"  , id);
    block.AppendColumn("name", name);

    client.Insert("test.numbers", block);
*/

































/*
// Initialize client connection.
clickhouse::Client client(ClientOptions().SetHost("localhost"));

// Create a table.
client.Execute("CREATE TABLE IF NOT EXISTS test.numbers (id UInt64, name String) ENGINE = Memory");

// Insert some values.
//{
Block block;

auto id = std::make_shared<ColumnUInt64>();
id->Append(1);
id->Append(7);

auto name = std::make_shared<ColumnString>();
name->Append("one");
name->Append("seven");

block.AppendColumn("id"  , id);
block.AppendColumn("name", name);

client.Insert("test.numbers", block);
//}

// Select values inserted in the previous step.
client.Select("SELECT id, name FROM test.numbers", [] (const Block& block))
{
    {
        for (size_t i = 0; i < block.GetRowCount(); ++i) {
            std::cout << block[0]->As<ColumnUInt64>()->At(i) << " "
                      << block[1]->As<ColumnString>()->At(i) << "\n";
        }
    }
}

// Delete table.
client.Execute("DROP TABLE test.numbers");
*/

