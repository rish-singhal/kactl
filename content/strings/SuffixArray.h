/**
 * Author: 罗穗骞, chilli
 * Date: 2019-04-11
 * License: Unknown
 * Source: Suffix array - a powerful tool for dealing with strings
 * (Chinese IOI National team training paper, 2009)
 * Description: Builds suffix array for a string.
 * \texttt{sa[i]} is the starting index of the suffix which
 * is $i$'th in the sorted suffix array.
 * The returned vector is of size $n+1$, and \texttt{sa[0] = n}.
 * The \texttt{lcp} array contains longest common prefixes for
 * neighbouring strings in the suffix array:
 * \texttt{lcp[i] = lcp(sa[i], sa[i-1])}, \texttt{lcp[0] = 0}.
 * The input string must not contain any zero bytes.
 * Time: O(n \log n)
 * Status: fuzz-tested
 */
#pragma once

struct SA {
    int n;
    // ht[i] = lcp(suffix[sa[i]], suffix[sa[i-1]])
    int rk[N], sa[N], ht[N];

    void build(string str) {
        n = str.length();
        str = " " + str;
        static int set[N], a[N];
        for(int i = 1; i <= n; i++) set[i] = str[i];
        sort(set + 1, set + n + 1);
        int *end = unique(set + 1, set + n + 1);
        for(int i = 1; i <= n; i++) a[i] = (int)(lower_bound(set + 1, end, str[i]) - set);

        static int fir[N], sec[N], tmp[N], buc[N];
        fill(buc, buc + n + 1, 0);
        for(int i = 1; i <= n; i++) buc[a[i]]++;
        for(int i = 1; i <= n; i++) buc[i] += buc[i-1];
        for(int i = 1; i <= n; i++) rk[i] = buc[a[i]-1] + 1;

        for(int t = 1; t <= n; t *= 2) {
            for(int i = 1; i <= n; i++) fir[i] = rk[i];
            for(int i = 1; i <= n; i++) sec[i] = i + t > n ? 0 : rk[i + t];

            fill(buc, buc + n + 1, 0);
            for(int i = 1; i <= n; i++) buc[sec[i]]++;
            for(int i = 1; i <= n; i++) buc[i] += buc[i - 1];
            for(int i = 1; i <= n; i++) tmp[n - (--buc[sec[i]])] = i;

            fill(buc, buc + n + 1, 0);
            for(int i = 1; i <= n; i++) buc[fir[i]]++;
            for(int i = 1; i <= n; i++) buc[i] += buc[i - 1];
            for(int j = 1, i; j <= n; j++) i = tmp[j], sa[buc[fir[i]]--] = i;

            bool unique = true;
            for (int j = 1, i, last = 0; j <= n; j++)
            {
                i = sa[j];
                if (!last) rk[i] = 1;
                else if (fir[i] == fir[last] && sec[i] == sec[last]) rk[i] = rk[last], unique = false;
                else rk[i] = rk[last] + 1;

                last = i;
            }

            if (unique) break;
        }
        for(int i = 1, k = 0; i <= n; i++) {
            if(rk[i] == 1) k = 0;
            else {
                if(k > 0) k--;
                int j = sa[rk[i]-1];
                while(i + k <= n && j + k <= n && a[i + k] == a[j + k]) k++;
            }
            ht[rk[i]] = k;
        }
    }
} sa;
