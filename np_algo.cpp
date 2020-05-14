#pragma GCC optimize("Ofast" , "sse4" , "sse3" , "sse2" , "unroll-loops")
#include <iostream>
#include <ctime>
#include <random>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <math.h>
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
#define SZ 20005
const int GEN_N = 7;
const int GEN_K = 7;
vector<int> gr[25005];
int a[25005];
int f[25005][SZ]; // f[v][c] is a maximum number of happy vertices in subtree of v if color of v is c
int g[25005][SZ]; // g[v][c] is a maximum number of happy vertices in subtree of v if color of v is c and is v is happy
int mf[25005]; // max value of f over all colors
// answer is maximum number of happy vertices = max(f[root][c] , g[root][c]) over all c
int n , k;
void dfs(int u , int p = -1)
{
    // cout << u << "\n";
    if(p != -1)
    {
        gr[u].erase(find(gr[u].begin() , gr[u].end() , p));
    }
    for(int v : gr[u])
    {
        dfs(v , u);
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
    //    sort(g[u], g[u] + k);
    //    sort(f[u], f[u] + k);
    for(int i = 0; i < k; i++)
    {
        mf[u] = max(f[u][i] , mf[u]);
    }
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
    //    n = rand() % GEN_N + 1;
    //    k = rand() % GEN_K + 1;
    for(int i = 0; i < n; i++)
    {
        a[i] = rand() % k;
        if(rand() % 2 == 0) a[i] = -1;
    }
    gen_tree(n);
}
vector<long double> tm;
vector<int> ks;
void get_time()
{
    int i = 25000;
    for(int j = 1; j <= SZ; j += 50)
    {
        n = i;
        k = j;
        gen(i,j);
        double clo = (double)(clock());
        long double prev_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        dfs(0, -1);
        long double cur_time = (double) ((clock()) / (double)CLOCKS_PER_SEC) ;
        if(prev_time > 0)
        {
            //cout << j << " " << cur_time - prev_time << "\n";
            tm.pb(cur_time - prev_time);
            ks.pb(j);
        }
        prev_time = cur_time ;
    }
    cout << "[";
    for(auto x : ks) cout << x << ",";
    cout << "]";
    cout << "\n";
    cout << "[";
    for(auto x : tm) cout << x << ",";
    cout << "]";
    cout << "\n";
}
int main()
{
    srand(228);
    get_time();
    //cout << solve() << " ";
    int ans = mf[0];
    for(int i = 0; i < k; i++)
    {
        ans = max(ans , g[0][i]);
    }
    cout << ans << "\n";
}
