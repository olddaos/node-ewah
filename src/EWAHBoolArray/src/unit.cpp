#include "ewah.h"

#include <stdlib.h>
#include <fstream>

using namespace std;

static string testfailed = "---\ntest failed.\n\n\n\n\n\n";


// for Microsoft compilers
#if _MSC_VER >= 1400
#define unlink _unlink
#endif

template<class uword>
bool testRunningLengthWord() {
    cout << "[testing RunningLengthWord]" << endl;
    bool isOk(true);
    uword somenumber(0xABCD);
    RunningLengthWord<uword> rlw(somenumber);
    rlw.setRunningBit(true);
    if (rlw.getRunningBit() != true) {
        cout << "failed to set the running bit " << sizeof (uword) << endl;
        isOk = false;
    }
    for (uword myrl = 0; myrl <= RunningLengthWord<uword>::largestrunninglengthcount; myrl+=RunningLengthWord<uword>::largestrunninglengthcount/10) {
        rlw.setRunningLength(myrl);
        if (rlw.getRunningBit() != true) {
            cout << "failed to set the running bit (2) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getRunningLength() != myrl) {
            cout << "failed to set the running length " << sizeof (uword) << endl;
            isOk = false;
        }
    }
    rlw.setRunningLength(12);
    for (uword mylw = 0; mylw <= RunningLengthWord<uword>::largestliteralcount; mylw+=RunningLengthWord<uword>::largestliteralcount/10) {
        rlw.setNumberOfLiteralWords(mylw);
        if (rlw.getRunningBit() != true) {
            cout << "failed to set the running bit (3) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getRunningLength() != 12) {
            cout << "failed to set the running length (2) " << sizeof (uword) << endl;
            isOk = false;
        }
        if (rlw.getNumberOfLiteralWords() != mylw) {
            cout << "failed to set the LiteralWords " <<mylw << " "<<sizeof (uword)<<" "<<rlw.getNumberOfLiteralWords() << endl;
            isOk = false;
        }
    }
    rlw.setNumberOfLiteralWords(43);
    rlw.setRunningBit(false);
    if (rlw.getRunningBit() != false) {
        cout << "failed to set the running bit (4) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (rlw.getRunningLength() != 12) {
        cout << "failed to set the running length (3) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (rlw.getNumberOfLiteralWords() != 43) {
        cout << "failed to set the LiteralWords (2) " << sizeof (uword) << endl;
        isOk = false;
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}


template <class uword>
bool testEWAHBoolArrayAppend() {
    cout << "[testing EWAHBoolArrayAppend]" << endl;
    bool isOk(true);
    uword zero = 0;
    uword specialval = 1UL + (1UL << 4)+(static_cast<uword>(1) << (sizeof(uword)*8-1));
    uword notzero = ~zero;
    EWAHBoolArray<uword> myarray1;
    BoolArray<uword> ba1;
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(zero);
    ba1.add(zero);
    myarray1.add(specialval);
    ba1.add(specialval);
    myarray1.add(specialval);
    ba1.add(specialval);
    myarray1.add(notzero);
    ba1.add(notzero);
    myarray1.add(zero);
    ba1.add(zero);
    EWAHBoolArray<uword> myarray2;
    BoolArray<uword> ba2;
    myarray2.add(notzero);
    ba2.add(notzero);
    myarray2.add(zero);
    ba2.add(zero);
    myarray2.add(notzero);
    ba2.add(notzero);
    myarray2.add(specialval);
    ba2.add(specialval);
    myarray2.add(specialval);
    ba2.add(specialval);
    myarray2.add(notzero);
    ba2.add(notzero);
    BoolArray<uword> aggregate1(ba1);
    BoolArray<uword> aggregate2(ba2);
    aggregate1.append(ba2);
    aggregate2.append(ba1);
    EWAHBoolArray<uword> caggregate1;
    caggregate1.append(myarray1);
    EWAHBoolArray<uword> caggregate2;
    caggregate2.append(myarray2);
    caggregate1.append(myarray2);
    caggregate2.append(myarray1);
    if (caggregate1 != aggregate1) {
        cout << "aggregate 1 failed" << endl;
        isOk = false;
    }
    if (caggregate2 != aggregate2) {
        cout << "aggregate 2 failed" << endl;
        isOk = false;
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}



// unit test contributed by Joerg Bukowski ✆
template<class uword>
bool testJoergBukowski() {
    cout << "[testing JoergBukowski]" << endl;
    bool isOk(true);
    vector<uint> positions;
    positions.push_back(0);
    positions.push_back(36778);
    positions.push_back(51863);
    positions.push_back(134946);
    positions.push_back(137330);
    positions.push_back(147726);
    positions.push_back(147990);
    positions.push_back(151884);
    positions.push_back(156404);
    positions.push_back(158486);
    positions.push_back(159622);
    positions.push_back(163159);
    positions.push_back(164599);
    string indexfile("testingewahboolarray.bin");
    ::unlink(indexfile.c_str());
    EWAHBoolArray<uword> myarray;
    for(vector<uint>::const_iterator i = positions.begin(); i!=positions.end(); ++i) {
    	myarray.set(*i);
	    ofstream out(indexfile.c_str(), ios::out | ios::binary);
	    myarray.write(out);
    	out.close();
    	EWAHBoolArray<uword> recovered;
    	ifstream in(indexfile.c_str(),ios::binary);
    	recovered.read(in);
    	in.close();
    	vector<uint> vals;
    	recovered.appendSetBits(vals);
    	if(vals.size()!= static_cast<size_t>(i-positions.begin()+1)) {
    	  cout<<"failed to recover right number"<<endl; 
    	  isOk= false;
    	}
    	if(!equal(vals.begin(),vals.end(),positions.begin())) {
    	  cout<<"failed to recover"<<endl; 
    	  isOk= false;
    	}
    	vals.clear();
    	for(typename EWAHBoolArray<uword>::const_iterator j = recovered.begin();
    	  j!= recovered.end(); ++j)
    	  vals.push_back(static_cast<uint>(*j));
    	if(vals.size()!= static_cast<size_t>(i-positions.begin()+1)) {
    	  cout<<"failed to recover right number -- iterator"<<endl; 
    	  isOk= false;
    	}
    	if(!equal(vals.begin(),vals.end(),positions.begin())) {
    	  cout<<"failed to recover -- iterator"<<endl; 
    	  isOk= false;
    	}
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}

// unit test contributed by Phong Tran
bool testPhongTran() {
    cout << "[testing PhongTran]" << endl;
    bool isOk(true);
    EWAHBoolArray<uword32> myarray;
    for(uword32 x = 0; x< 10000; x++) {
        myarray.add(x);
    }
    string indexfile("testingewahboolarray.bin");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    myarray.write(out);
    out.close();
    EWAHBoolArray<uword32> lmyarray;
    ifstream in(indexfile.c_str(),ios::binary);
    lmyarray.read(in);
    in.close();
    EWAHBoolArrayIterator<uword32> i = myarray.uncompress();
    EWAHBoolArrayIterator<uword32> j = lmyarray.uncompress();
    while (i.hasNext() or j.hasNext()) {
        if ((!j.hasNext()) or (!i.hasNext())) {
            cout<<"the two arrays don't have the same size?"<<endl;
            isOk = false;
            break;
        }
        uword32 val = i.next();
        uword32 val2 = j.next();
        if (val != val2) {
            cout<<"the two arrays differ" << endl;
            isOk = false;
            break;
        }
    }
    
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}


// another unit test contributed by Phong Tran
template<class uword>
bool testPhongTran2() {
    cout << "[testing PhongTran2]" << endl;
    bool isOk(true);
    uword iTotal = static_cast<uword>(1000); // when 1000 does not fit in uword, then it will be casted
    EWAHBoolArray<uword> myarray;
    for(uword x = static_cast<uword>(100); x< iTotal; x++)  {
        myarray.add(x);
    }
    string indexfile("testingewahboolarray.bin");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    myarray.write(out);
    out.close();
    EWAHBoolArray<uword> lmyarray;
    ifstream in(indexfile.c_str(),ios::binary);
    lmyarray.read(in);
    in.close();
    if (!(myarray == lmyarray)) {
        cout << "bad news, they are not equal" << endl;
        cout << "size in bits: " << myarray.sizeInBits() << " vs. " << lmyarray.sizeInBits() << endl;
        isOk = false;
    }
    EWAHBoolArrayIterator<uword> i = myarray.uncompress();
    EWAHBoolArrayIterator<uword> j = lmyarray.uncompress();
    while (i.hasNext())    {
        if (!j.hasNext())    {
            cout<<"the two arrays don't have the same size?"<<endl;
            isOk = false;
            break;
        }
        uword val = i.next();
        uword val2 = j.next();
        if (val != val2)    {
            cout<<"the two arrays differ " << endl;
            isOk = false;
        }
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}


template<class uword>
bool testEWAHBoolArray() {
    cout << "[testing EWAHBoolArray]" << endl;
    bool isOk(true);
    EWAHBoolArray<uword> myarray;
    BoolArray<uword> ba(10 * sizeof (uword) * 8);
    uword zero = 0;
    uword notzero = ~zero;
    myarray.add(zero);
    ba.setWord(0, zero);
    myarray.add(zero);
    ba.setWord(1, zero);
    myarray.add(zero);
    ba.setWord(2, zero);
    uword specialval = 1UL + (1UL << 4)+(static_cast<uword>(1) << (sizeof(uword)*8-1));
    myarray.add(specialval);
    ba.setWord(3, specialval);
    myarray.add(notzero);
    ba.setWord(4, notzero);
    myarray.add(notzero);
    ba.setWord(5, notzero);
    myarray.add(notzero);
    ba.setWord(6, notzero);
    myarray.add(notzero);
    ba.setWord(7, notzero);
    myarray.add(specialval);
    ba.setWord(8, specialval);
    myarray.add(zero);
    ba.setWord(9, zero);
    if (myarray.sizeInBits() != 10 * sizeof (uword) * 8) {
        cout << "expected " << 10 * sizeof (uword) * 8 << " bits but found " << myarray.sizeInBits() << endl;
        isOk = false;
    }
    string indexfile("testingewahboolarray.bin");
    ::unlink(indexfile.c_str());
    ofstream out(indexfile.c_str(), ios::out | ios::binary);
    myarray.write(out);
    out.close();
    EWAHBoolArray<uword> lmyarray;
    ifstream in(indexfile.c_str(),ios::binary);
    lmyarray.read(in);
    in.close();
    if (!(myarray == lmyarray)) {
        cout << "bad news, they are not equal" << endl;
        cout << "size in bits: " << myarray.sizeInBits() << " vs. " << lmyarray.sizeInBits() << endl;
        isOk = false;
    }
    EWAHBoolArrayIterator<uword> i = myarray.uncompress();
    EWAHBoolArrayIterator<uword> j = lmyarray.uncompress();
    uint k = 0;
    while (i.hasNext()) {
        if (!j.hasNext()) {
            cout<<"the two arrays don't have the same size?"<<endl;
            isOk = false;
            break;
        }
        uword val = i.next();
        uword val2 = j.next();
        uword valref = ba.getWord(k++);
        if (val != valref) {
            cout<<"the two arrays differ from uncompressed array at "<<k<<" "<< val<< " "<< val2<<" " <<valref << endl;
            isOk = false;
        }
        if (val != val2) {
            cout<<"the two arrays differ at "<<k<<" "<< val<< " "<< val2<<" " <<valref << endl;
            isOk = false;
        }
    }
    if (isOk) ::unlink(indexfile.c_str());
    if (!isOk) cout << testfailed << endl;
    return isOk;
}



template<class uword>
bool testEWAHBoolArrayLogical() {
    cout << "[testing EWAHBoolArrayLogical]" << endl;
    bool isOk(true);
    EWAHBoolArray<uword> myarray1;
    EWAHBoolArray<uword> myarray2;
    const uint N=15;
    uword allones = static_cast<uword>(~0L);
    uword x1[N] = {1,54,24,145,0,0,0,allones,allones,allones,43,0,0,0,1};
    uword x2[N] = {allones,0,0,0,0,0,0,0,allones,allones,allones,0,4,0,0};
    uword xand[N];
    uword xxor[N];
    for(uint k = 0; k < N; ++k) {
        myarray1.add(x1[k]);
        myarray2.add(x2[k]);
        xand[k] = x1[k] & x2[k];
        xxor[k] = x1[k] | x2[k];
    }
    EWAHBoolArray<uword> myand;
    EWAHBoolArray<uword> mysparseand;
    EWAHBoolArray<uword> myor;
    myarray1.rawlogicaland(myarray2,myand);
    myarray1.sparselogicaland(myarray2,mysparseand);
    myarray1.rawlogicalor(myarray2,myor);
    EWAHBoolArrayIterator<uword> i = myand.uncompress();
    EWAHBoolArrayIterator<uword> ii = mysparseand.uncompress();
    EWAHBoolArrayIterator<uword> j = myor.uncompress();
    EWAHBoolArrayIterator<uword> it1 = myarray1.uncompress();
    EWAHBoolArrayIterator<uword> it2 = myarray2.uncompress();
    for(uint k = 0; k <N; ++k) {
        const uword m1 = it1.next();
        const uword m2 = it2.next();
        if(!i.hasNext()) {
            cout<<"type 1 error"<<endl;
            isOk=false;
            break;
        }
        if(!ii.hasNext()) {
            cout<<"type 2 error"<<endl;
            isOk=false;
            break;
        }
        if(!j.hasNext()) {
            cout<<"type 3 error"<<endl;
            isOk=false;
            break;
        }
        if(i.next()!= xand[k]) {
            cout<<"type 4 error"<<endl;
            isOk=false;
            break;
        }
        if(ii.next()!= xand[k]) {
            cout<<"type 5 error"<<endl;
            isOk=false;
            break;
        }
        const uword jor = j.next();
        if(jor!= xxor[k]) {
            cout<<m1<<" or "<< m2<<" = "<< xxor[k] << " but got "<<jor <<endl;
            cout<<"type 6 error OR"<<endl;
            isOk=false;
            break;
        }
    }
    if (!isOk) cout << testfailed << endl;
    return isOk;
}



void tellmeaboutmachine() {
    cout << "number of bytes in ostream::pos_type = " << sizeof (ostream::pos_type) << endl;
    cout << "number of bytes in size_t = " << sizeof (size_t) << endl;
    if (sizeof (ulong) > 4)
        cout << "you have a 64-bit machine" << endl;
    else
        cout << "you have a 32-bit machine (if not, consider recompiling with the flag -m64)" << endl;
#if  __LITTLE_ENDIAN__
    cout << "you have little endian machine" << endl;
#endif
#if  __BIG_ENDIAN__
    cout << "you have a big endian machine" << endl;
#endif
#if __CHAR_BIT__
    if (__CHAR_BIT__ != 8)
        cout << "on your machine, chars don't have 8bits???" << endl;
#endif
}




int main(void) {
    int failures = 0;
    if (!testPhongTran()) ++failures;

    if (!testPhongTran2<uword16 > ())++failures;
    if (!testPhongTran2<uword32 > ())++failures;
    if (!testPhongTran2<uword64 > ())++failures;

    if (!testJoergBukowski<uword16 > ())++failures;
    if (!testJoergBukowski<uword32 > ())++failures;
    if (!testJoergBukowski<uword64 > ())++failures;


    if (!testRunningLengthWord<uword16 > ())++failures;
    if (!testRunningLengthWord<uword32 > ())++failures;
    if (!testRunningLengthWord<uword64 > ())++failures;
    if (!testEWAHBoolArray<uword16 > ())++failures;
    if (!testEWAHBoolArray<uword32 > ())++failures;
    if (!testEWAHBoolArray<uword64 > ())++failures;

    if (!testEWAHBoolArrayLogical<uword16 > ())++failures;
    if (!testEWAHBoolArrayLogical<uword32 > ())++failures;
    if (!testEWAHBoolArrayLogical<uword64 > ())++failures;

    if (!testEWAHBoolArrayAppend<uword16 > ())++failures;
    if (!testEWAHBoolArrayAppend<uword32 > ())++failures;
    if (!testEWAHBoolArrayAppend<uword64 > ())++failures;
    tellmeaboutmachine();
    if (failures == 0) {
        cout << "Your code is ok." << endl;
        return EXIT_SUCCESS;
    }
    cout << "Got " << failures << " failed tests!" << endl;
    return EXIT_FAILURE;
}
