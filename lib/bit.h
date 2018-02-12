#include<cstdint>
namespace{
	class Bit{
		private:
			int64_t size;
			int64_t *data;

		public:
			Bit(int64_t n){
				int64_t newsize = 1;
				while(newsize < n)  newsize = newsize << 1;
				size = newsize + 1;
				data = new int64_t[size];
				for(int64_t i = 0; i < size; ++i){
					data[i] = 0;
				}
			}

			int64_t sum(long long pos){
				pos++;
				int64_t ret = 0;
				while(pos > 0){
					ret += data[pos];
					pos -= pos & -pos;
				}
				return ret;
			}

			void update(int64_t pos, long long newValue){
				int64_t delta = newValue - data[pos];
				pos++;
				while(pos <= size){
					data[pos] += delta;
					pos += pos & -pos;
				}
			}
	};
}
