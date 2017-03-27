#ifndef FAKES_TGSW_FFT_H
#define FAKES_TGSW_FFT_H

namespace {

    // Fake TLWE structure 
    struct FakeTGswFFT {
	static const int FAKE_TYPE_UID = 574569852; // precaution: distinguish fakes from trues
	const int fake_uid;
	IntPolynomial* message;
	double current_variance;

	//this padding is here to make sure that FakeTLwe and TLweSample have the same size
	char unused_padding[sizeof(TGswSampleFFT)-sizeof(long)-sizeof(IntPolynomial*)-sizeof(double)];

	// construct
	FakeTGSW(int N):fake_uid(FAKE_TYPE_UID) {
	    message = new_IntPolynomial(N);
	    current_variance = 0.;
	}

	// delete
	~FakeTGsw() {
	    if (fake_uid!=FAKE_TYPE_UID) abort();
	    delete_IntPolynomial(message);
	}
	FakeTGsw(const FakeTGsw&)=delete;
	void operator=(const FakeTGsw&)=delete;
    };



    // At compile time, we verify that the two structures have exactly the same size
    static_assert (sizeof(FakeTGswFFT) == sizeof(TGswSampleFFT), "Error: Size is not correct");

    // fake functions 
    inline FakeTGswFFT* fake(TGswSampleFFT* sample) {
	FakeTGswFFT* reps = (FakeTGswFFT*) sample;
	if (reps->fake_uid!=FakeTGswFFT::FAKE_TYPE_UID) abort();
	return reps; 
    }
    inline const FakeTGswFFT* fake(const TGswSampleFFT* sample) {
	const FakeTGswFFT* reps = (const FakeTGswFFT*) sample;
	if (reps->fake_uid!=FakeTGswFFT::FAKE_TYPE_UID) abort();
	return reps; 
    }


    //-----------------------------------------
    //  constructor/destructor
    //-----------------------------------------

    inline TGswSampleFFT* fake_new_TGswSampleFFT_array(int nbelts, const TGswParams* params) {
        int N = params->N;
        FakeTGswFFT* reps = (FakeTGswFFT*) malloc(nbelts*sizeof(FakeTGswFFT));
        for (int i=0; i<nbelts; i++) new(reps+i) FakeTGswFFT(params);
        return (TGswSampleFFT*) reps;
    }

#define USE_FAKE_new_TGswSampleFFT_array \
    inline TGswSampleFFT* new_TGswSampleFFT_array(int nbelts, const TGswParams* params) { \
        return fake_new_TGswSampleFFT_array(nbelts,params); \
    }

    inline void fake_delete_TGswSampleFFT_array(int nbelts, TGswSampleFFT* sample) {
        FakeTGswFFT* arr = fake(sample);
        for (int i=0; i<nbelts; i++) (arr+i)->~FakeTGswFFT();
        free(arr);
    }

    // 
#define USE_FAKE_delete_TGswSampleFFT_array \
    inline void delete_TGswSampleFFT_array(int nbelts, TGswSampleFFT* samples) { \
        fake_delete_TGswSampleFFT_array(nbelts,samples); \
    }



    inline TGswSampleFFT* fake_new_TGswSampleFFT(const TGswParams* params) {
        int N = params->N;
        FakeTGswFFT* reps = (FakeTGswFFT*) malloc(sizeof(FakeTGswFFT));
        new(reps) FakeTGswFFT(params);
        return (TGswSampleFFT*) reps;
    }

    // 
#define USE_FAKE_new_TGswSampleFFT \
    inline TGswSampleFFT* new_TGswSampleFFT(const TGswParams* params) { \
        return fake_new_TGswSampleFFT(params); \
    }

    inline void fake_delete_TGswSampleFFT(TGswSampleFFT* sample) {
        FakeTGswFFT* ptr = fake(sample);
        (ptr)->~FakeTGswFFT();
        free(ptr);
    }

    // 
#define USE_FAKE_delete_TGswSampleFFT \
    inline void delete_TGswSampleFFT(TGswSampleFFT* sample) { \
	fake_delete_TGswSampleFFT(sample); \
    }


    //-----------------------------------------------
    //  functions
    //-----------------------------------------------


    // Computes the inverse FFT of the coefficients of the TLWE sample
    inline void fake_tGswToFFTConvert(TGswSampleFFT* result, const TGswSample* source, const TGswParams* params){
	const FakeTGsw* fs = fake(source);
	FakeTGswFFT* fres = fake(result);

	IntPolynomial_copy(fres->message, fs->message);
	fres->current_variance=fs->current_variance;
    }

#define USE_FAKE_tGswToFFTConvert \
    inline void tGswToFFTConvert(TGswSampleFFT* result, const TGswSample* source, const TGswParams* params){ \
	fake_tGswToFFTConvert(result, source, params); \
    }


    // Computes the FFT of the coefficients of the TLWEfft sample
    inline void fake_tGswFromFFTConvert(TGswSample* result, const TGswSampleFFT* source, const TGswParams* params){
	FakeTGswFFT* fs = fake(source);
	FakeTGsw* fres = fake(result);

	IntPolynomial_copy(fres->message, fs->message);
	fres->current_variance=fs->current_variance;
    }


#define  tGswFromFFTConvert \
    inline void tGswFromFFTConvert(TGswSample* result, const TGswSampleFFT* source, const TGswParams* params) { \
	fake_tGswFromFFTConvert(result, source, params); \
    }

    //Arithmetic operations on TGsw samples
    /** result = (0,0) */
    inline void fake_tGswFFTClear(TGswSampleFFT* result, const TGswParams* params){
	FakeTGswFFT* fres = fake(result);

	IntPolynomial_clear(fres->message);
	fres->current_variance=0;
    }

#define USE_FAKE_tGswFFTClear \
    inline void tGswFFTClear(TGswSampleFFT* result, const TGswParams* params) { \
	fake_tGswFFTClear(result, params); \
    }





    // result = result + H
    inline void fake_tGswFFTAddH(TGswSampleFFT* result, const TGswParams* params) {
	FakeTGswFFT* fres = fake(result);
	result->message->coefs[0]++;
	//variance is preserved
    }

#define USE_FAKE_tGswFFTAddH \
    inline void tGswFFTAddH(TGswSampleFFT* result, const TGswParams* params) { \
	fake_tGswFFTAddH(result, params); \
    }	

    // result = list of TLWE (0,0)
    inline void tGswFFTClear(TGswSampleFFT* result, const TGswParams* params) {
	FakeTGswFFT* fres = fake(result);
	intPolynomialClear(result->message);
	result->current_variance=0;
    }    

#define USE_FAKE_tGswFFTClear \
    inline void tGswFFTClear(TGswSampleFFT* result, const TGswParams* params) { \
	fake_tGswFFTClear(result, params); \
    }

    // External product (*): accum = gsw (*) accum 
    inline void fake_tGswFFTExternMulToTLwe(TLweSample* accum, TGswSampleFFT* gsw, const TGswParams* params) {
	const int N = params->tlwe_params->N;
	const FakeTGswFFT* fgsw = fake(gsw);
	const FakeTLwe* faccum = fake(accum);
	TorusPolynomial* tmp = new_TorusPolynomial(N);
	torusPolynomialMultKaratsuba(tmp,fgsw->message,faccum->message);
	TorusPolynomial_copy(faccum->message, tmp);
	delete_TorusPolynomial(tmp);
    }

#define USE_FAKE_tGswFFTExternMulToTLwe \
    inline void tGswFFTExternMulToTLwe(TLweSample* accum, TGswSampleFFT* gsw, const TGswParams* params) { \
	fake_tGswFFTExternMulToTLwe(accum, gsw, params); \
    }

    // result = (X^ai -1)*bki  
    inline void fake_tGswFFTMulByXaiMinusOne(TGswSampleFFT* result, const int ai, const TGswSampleFFT* bki, const TGswParams* params) {
	FakeTGswFFT* fres = fake(result);
	const FakeTGswFFT* fbki = fake(bki);
	intPolynomialMulByXaiMinusOne(fres->message,ai,fbki->message);
    }

#define USE_FAKE_tGswFFTMulByXaiMinusOne \
    inline void tGswFFTMulByXaiMinusOne(TGswSampleFFT* result, const int ai, const TGswSampleFFT* bki, const TGswParams* params) { \
    fake_tGswFFTMulByXaiMinusOne(result, ai, bki, params); \
    }

}

#endif // FAKES_TGSW_FFT_H