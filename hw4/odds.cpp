void removeOdds(vector<int>& v)
{

	for (int i = 0; i < v.size();) {
		if (v[i] % 2 != 0) {
			v.erase(v.begin() + i);
		}
		else {
			i++;
		}
	}
}