class Bit{
    private:
        long long size;
        long long *data;

    public:
        Bit(long long n){
			long long newsize = 1;
			while(newsize < n)  newsize = newsize << 1;
			size = newsize + 1;
			data = new long long[size];
			for(long long i = 0; i < size; ++i){
				data[i] = 0;
			}
		}
        
		long long sum(long long pos){
			long long ret = 0;
			while(pos > 0){
				ret += data[pos];
				pos -= pos & -pos;
			}
			return ret;
		}

		void add(long long pos, long long delta){
			while(pos <= size){
				data[pos] += delta;
				pos += pos & -pos;
			}
		}
};
