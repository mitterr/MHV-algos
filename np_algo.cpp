#pragma GCC optimize("Ofast" , "sse4" , "sse3" , "sse2" , "unroll-loops")
#include <iostream>
#include <Python.h>
#include <ctime>
#include <random>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <math.h>
#include <Python.h>
#include <string>
#include <set>
#include <iomanip>
#include <bitset>
#include <random>
#include <complex>
#include <random>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
using namespace std;
#define pb push_back
typedef long long ll;
const int SZ = 25005;
const int GEN_N = 7;
const int GEN_K = 7;
vector<int> gr[25005];
int a[SZ];
int f[SZ][SZ]; // f[v][c] is a maximum number of happy vertices in subtree of v if color of v is c
int g[SZ][SZ]; // g[v][c] is a maximum number of happy vertices in subtree of v if color of v is c and is v is happy
int Th[SZ][SZ]; // Th[v][c] is a maximum number of happy vertices in subtree of v if color of v is c and is v is happy
int Tu[SZ][SZ]; // Tu[v][c] is a maximum number of happy vertices in subtree of v if color of v is c and is v is unhappy
int Tall[SZ][SZ]; // Tall[v][c] is a maximum number of happy vertices in subtree of v if color of v is c and is v is unhappy

int mf[SZ]; // max value of f over all colors
// answer is maximum number of happy vertices = max(f[root][c] , g[root][c]) over all c
int n , k;
void solve_nk(int u , int p = -1)
{
    // cout << u << "\n";
    if(p != -1)
    {
        gr[u].erase(find(gr[u].begin() , gr[u].end() , p));
    }
    for(int v : gr[u])
    {
        solve_nk(v , u);
    }
    for(int i = 0; i < k; i++)
    {
        if(a[u]>= 0 && a[u] != i)
        {
            continue;
        }
        g[u][i] = 1;
        f[u][i] = 0;
    }
    for(int v : gr[u])
    {
        for(int i = 0; i < k; i++)
        {
            if(a[u]>= 0 && a[u] != i)
            {
                continue;
            }
            f[u][i] += max(mf[v] , g[v][i]);
            g[u][i] += max(f[v][i] , g[v][i]);
        }
    }
    for(int i = 0; i < k; i++)
    {
        mf[u] = max(f[u][i] , mf[u]);
    }
}


void solve_nk_log_k(int u , int p = -1)
{
    if(p != -1)
    {
        gr[u].erase(find(gr[u].begin() , gr[u].end() , p));
    }
    for(int v : gr[u])
    {
        solve_nk_log_k(v , u);
    }
    for(int i = 0; i < k; i++)
    {
        if(a[u]>= 0 && a[u] != i)
        {
            continue;
        }
        Th[u][i] = 1;
        Tu[u][i] = 0;
    }
    for(int v : gr[u])
    {
        for(int i = 0; i < k; i++)
        {
            if(a[u]>= 0 && a[u] != i)
            {
                continue;
            }
            Th[u][i] += max(Th[0][i], Tu[v][i]);
            Tu[u][i] += max(Th[v][i], Tu[v][i]);
            Tall[u][i] = max(Th[u][i], Tu[v][i]);
            
        }
    }
    sort(Th[u], Th[u] + k); // sort to get ordered values to get fast maximum
    sort(Tu[u], Tu[u] + k);  // sort to get ordered values to get fast maximum
}

vector<int> get_mask(int j)
{
    vector<int> mask;
    while (j > 0)
    {
        mask.push_back(j % k);
        j /= k;
    }
    while (mask.size() < n) {
        mask.pb(0);
    }
    reverse(mask.begin(), mask.end());
    for(int i = 0; i < n; i++)
    {
        if(a[i] >= 0)
            mask[i] = a[i];
    }
    return mask;
}

bool try_dfs(int u , vector<int> &cl )
{
    bool flag = 1;
    for(auto v : gr[u])
    {
        flag &= (cl[v] == cl[u]);
        if(!flag)
        {
            return 0;
        }
    }
    return 1;
}
int solve()
{
    int mx = 0;
    if( k == 1) return n;
    for(int i = 0; i <= pow(k , n); i++)
    {
        int ans = 0;
        vector<int> p = get_mask(i);
        for(int j = 0; j < n; j++)
            ans += try_dfs(j, p);
        mx = max(ans , mx);
    }
    return mx;
}
void gen_tree(int n)
{
    for (int i = 2; i <= n; ++i)
    {
        int p = rand() % (i - 1);
        gr[p].pb(i - 1);
        gr[i - 1].pb(p);
    }
}

void relax(int n, int k)
{
    for(int i = 0; i < n + 1; i++)
    {
        gr[i].clear();
    }
    memset(a, 0, sizeof(a));
    for(int i = 0; i < n + 1; i++)
        for(int j = 0; j < k + 1; j++)
            g[i][j] = f[i][j] = mf[i] = -1;
}

void gen(int n , int k)
{
    relax(n, k);
    for(int i = 0; i < n; i++)
    {
        a[i] = rand() % k;
        if(rand() % 2 == 0) a[i] = -1;
    }
    gen_tree(n);
}

vector<long double> tm;
vector<int> ks;

pair<vector<double>, vector<double>> get_time_for_fixed_N(int n, int freq)
{
    int i = n;
    vector<double> times_nk, times_nk_log_k;
    for(int j = 1; j <= SZ; j += freq)
    {
        n = i;
        k = j;
        gen(i,j);
        double clo = (double)(clock());
        long double prev_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        solve_nk(0, -1);
        long double cur_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        times_nk.pb(cur_time - prev_time);
        prev_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        solve_nk_log_k(0, -1);
        cur_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        times_nk_log_k.pb(cur_time - prev_time);
    }
    return make_pair(times_nk, times_nk_log_k);
}

void run_and_save_to_file(int n, int freq)
{
    freopen("input.txt","w", stdout);
    auto timelines = get_time_for_fixed_N(n, freq);
    for(auto ak: timelines.first) cout << vec << " ";
    cout << "\n";
    for(auto bk: timelines.first) cout << vec << " ";
    cout << "\n" << freq << "\n";
    return;
}
void run_py_stats_scripts()
{
    Py_SetProgramName(argv[0]);  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleString("'sys.path.append("r_square_mean.py")');
    Py_Finalize();
}
int main(int argc,int argvx[])
{
    srand(228);
    n = atoi(argvx[1]);
    int freq = atoi(argvx[2]);
    run_and_save_to_file(n);
    run_py_stats_scripts();
}

