#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <tuple>
#include <algorithm>
#include <bitset>
#include <ctime>
#include <numeric>
#include <clocale>

using namespace std;

long long int get_prime(long long int min, long long int max);
long long int get_coprime(long long int phi);
tuple<long long int, long long int, long long int> extended_euclidean_algorithm(long long int a, long long int b);
long long int pow_mod(long long int base, long long int exp, long long int mod);
unsigned long long int getrandbits(long long int n);
long long int generate_random_in_range(long long min, long long max);

bool is_prime(long long int n)
{
    for (long long int i = 1; i < 5; i++)
    {
        long long int a = rand() % (n - 1) + 1;
        if (pow_mod(a, n - 1, n) != 1)
        {
            return false;
        }
    }
    return true;
}

long long int get_prime(long long int left, long long int right)
{
    while (true)
    {
        long long int p = left + rand() % (right - left + 1);
        if (is_prime(p))
        {
            return p;
        }
    }
}

long long int pow_mod(long long int base, long long int exp, long long int mod)
{
    long long int result = 1;
    base %= mod;

    if (base == 0) return 0;

    while (exp > 0)
    {
        if ((exp & 1) == 1)
        {
            result = (result * base) % mod;
        }
        exp >>= 1;
        base = (base * base) % mod;
    }

    return result;
}

tuple<long long int, long long int, long long int> extended_euclidean_algorithm(long long int a, long long int b)
{
    long long int u0 = a, u1 = 1, u2 = 0;
    long long int v0 = b, v1 = 0, v2 = 1;

    while (v0 != 0)
    {
        long long int q = u0 / v0;
        long long int t0 = u0 % v0, t1 = u1 - q * v1, t2 = u2 - q * v2;
        u0 = v0; u1 = v1; u2 = v2;
        v0 = t0; v1 = t1; v2 = t2;
    }

    return make_tuple(u0, u1, u2);
}

long long int get_coprime(long long int num)
{
    uint64_t res = 2 + rand() % (num - 2 + 1);
    while (gcd(num, res) != 1)
    {
        res = 2 + rand() % (num - 2 + 1);
    }
    return res;
}

tuple<vector<long long int>, vector<long long int>, vector<string>, long long int> read_graph(const string& filename)
{
    vector<long long int> from;
    vector<long long int> to;
    vector<string> colors;
    long long int vertex_num = 0;

    ifstream file(filename);
    if (file.is_open())
    {
        long long int edge_num;
        file >> vertex_num >> edge_num;
        for (long long int i = 0; i < edge_num; i++)
        {
            long long int fr, to_v;
            file >> fr >> to_v;
            from.push_back(fr);
            to.push_back(to_v);
        }

        string color;
        while (file >> color)
        {
            colors.push_back(color);
        }
    }
    return make_tuple(from, to, colors, vertex_num);
}

unsigned long long int getrandbits(long long int n)
{
    unsigned long long int result;

    for (long long int i = 0; i < n; i++)
    {
        result = (result << 1) | rand() % 2;
    }

    return result;
}

long long int generate_random_in_range(long long min, long long max)
{
    uint64_t range = max - min + 1;
    uint64_t random_number = ((uint64_t)rand() << 16) | rand();
    return min + (random_number % range);
}

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    auto [from, to, colors, vertex_num] = read_graph("input.txt");

    cout << "Количество вершин: " << vertex_num << "\nКоличество ребер: " << from.size() << "\n";

    //cout << "Граф содержит " << vertex_num << " вершин и " << from.size() << " ребер:\n";
    for (size_t i = 0; i < from.size(); i++)
    {
        cout << from[i] << " " << to[i] << "\n";
    }

    cout << "Раскраска графа: ";
    for (const auto& color : colors)
    {
        cout << color << " ";
    }
    cout << "\n";

    // Alice
    vector<string> color_name = { "R", "Y", "B" };
    vector<string> color_name_shuffle = color_name;
    while (color_name_shuffle == color_name)
    {
        shuffle(color_name_shuffle.begin(), color_name_shuffle.end(), mt19937{ random_device{}() });
    }

    vector<string> colors_shuffle(vertex_num);
    for (long long int i = 0; i < vertex_num; i++)
    {
        if (colors[i] == "R")
        {
            colors_shuffle[i] = color_name_shuffle[0];
        }
        else if (colors[i] == "Y")
        {
            colors_shuffle[i] = color_name_shuffle[1];
        }
        else if (colors[i] == "B")
        {
            colors_shuffle[i] = color_name_shuffle[2];
        }
    }

    cout << "Новая раскраска графа: ";
    for (const auto& color : colors_shuffle)
    {
        cout << color << " ";
    }
    cout << "\n";

    vector<long long int> r;
    for (const auto& color : colors_shuffle)
    {
        if (color == "R")
        {
            r.push_back((generate_random_in_range(100000, 10000000) >> 2) << 2);
        }
        else if (color == "Y")
        {
            r.push_back(((generate_random_in_range(100000, 10000000) >> 2) << 2) | 1);
        }
        else if (color == "B")
        {
            r.push_back(((generate_random_in_range(100000, 10000000) >> 2) << 2) | 2);
        }
    }

    cout << "Сгенерированные r = ";
    for (const auto& val : r)
    {
        cout << val << " ";
    }
    cout << "\n";

    vector<long long int> p(vertex_num), q(vertex_num), n(vertex_num), phi(vertex_num), d(vertex_num), c(vertex_num), Z(vertex_num);
    for (long long int i = 0; i < vertex_num; i++)
    {
        p[i] = get_prime(0, 1000000000);
        q[i] = get_prime(0, 1000000000);
        n[i] = p[i] * q[i];
        phi[i] = (p[i] - 1) * (q[i] - 1);
        d[i] = get_coprime(phi[i]);
        c[i] = get<1>(extended_euclidean_algorithm(d[i], phi[i]));
        while (c[i] < 0)
        {
            c[i] += phi[i];
        }
        Z[i] = pow_mod(r[i], d[i], n[i]);
    }

    for (int i = 0; i < Z.size(); i++)
    {
        cout << "Z[" << i << "] = " << Z[i] << endl;
    }
    cout << endl;

    for (int i = 0; i < c.size(); i++)
    {
        cout << "c[" << i << "] = " << c[i] << endl;
    }
    cout << endl;

    for (int i = 0; i < n.size(); i++)
    {
        cout << "n[" << i << "] = " << n[i] << endl;
    }
    cout << endl;

    for (int i = 0; i < d.size(); i++)
    {
        cout << "d[" << i << "] = " << d[i] << endl;
    }
    cout << endl;

    for (int i = 0; i < from.size(); i++)
    {
        long long int _Z1 = pow_mod(Z[from[i] - 1], c[from[i] - 1], n[from[i] - 1]);
        long long int _Z2 = pow_mod(Z[to[i] - 1], c[to[i] - 1], n[to[i] - 1]);
        if ((_Z1 & 3) != (_Z2 & 3))
        {
            cout << "Ребро: " << (i + 1) << "(" << from[i] - 1 << ", " << to[i] - 1 << ")" << ", младшие биты различны.\n";
        }
        else
        {
            cout << "Ребро: " << (i + 1) << "(" << from[i] - 1 << ", " << to[i] - 1 << ")" << ", младшие биты совпадают.\n";
                //<< bitset<32>(_Z1) << " | " << bitset<32>(_Z2) << "\n";
        }
    }

    return 0;
}
