#ifndef _EliteQuant_Common_OrderManager_H_
#define _EliteQuant_Common_OrderManager_H_
#include <string>
#include <map>
#include <mutex>
#include <atomic>
#include <Common/config.h>
#include <Common/Order/order.h>
#include <Common/Order/fill.h>
#include <Common/Order/orderstatus.h>
#include <Common/Util/util.h>

using namespace std;

namespace EliteQuant {
	extern long m_orderId; //definition is hereglobal order id, should be initialized by broker, 
	extern std::mutex oid_mtx;
	extern std::mutex orderStatus_mtx;

	// TODO: maintain order book
	class OrderManager {
	public:
		OrderManager();
		~OrderManager();				// release all the orders
		static OrderManager* pinstance_;
		static mutex instancelock_;
		static OrderManager& instance();

		//std::atomic_int _count = { 0 };
		int _count = 0;
		std::map<long, std::shared_ptr<Order>> _orders;
		std::map<long, long> _sents;		// signed order size
		std::map<long, long> _fills;       // signed filled size
		std::map<long, bool> _cancels;    // if cancelled
		mutex wlock;
		void reset();

		void placeOrder(std::shared_ptr<Order> o);		// put order under track
		void gotOrder(long oid);						// order acknowledged
		void gotFill(Fill& fill);
		void gotCancel(long oid);
		std::shared_ptr<Order> retrieveOrder(long oid);
		vector<std::shared_ptr<Order>> retrieveOrder(const string& fullsymbol);
		vector<std::shared_ptr<Order>> retrieveNonFilledOrderPtr();
		vector<std::shared_ptr<Order>> retrieveNonFilledOrderPtr(const string& fullsymbol);
		vector<long> retrieveNonFilledOrderId();
		vector<long> retrieveNonFilledOrderId(const string& fullsymbol);

		bool isEmpty();
		bool isTracked(long oid);
		bool isFilled(long oid);
		bool isCanceled(long oid);
		bool isCompleted(long oid);		// either filled or canceled
		bool hasPendingOrders();		// is all orders either filled or canceled?
	};
}

#endif  // _EliteQuant_Common_OrderManager_H_