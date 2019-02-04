class BooleanInputService_StaticConfig extends Config {
    GetByteArray() {
        return new Uint8Array([ 
            1, 
            this.Value ]);
    }
}

class BooleanInputServiceConfig extends Config {
    GetByteArray() {
        return new Uint8Array([ 
            2, ])
            .concatArray(new Uint8Array(new Uint16Array([this.Pin]).buffer))
            .concatArray(new Uint8Array([this.Inverted ]));
    }
}

class BooleanOutputServiceConfig extends Config {
    GetByteArray() {
        return new Uint8Array([ 
            1, ])
            .concatArray(new Uint8Array(new Uint16Array([this.Pin]).buffer))
            .concatArray(new Uint8Array([this.NormalOn, 
            this.HighZ ]));
    }
}

class ButtonService_PollingConfig extends Config {
    GetByteArray() {
        return new Uint8Array([ 
            1 ])
            .concatArray(this.BooleanInputServiceConfig.GetByteArray());
    }
}

class FloatInputService_Static extends Config {
    GetByteArray() {
        return new Uint8Array([ 
            1 ]) 
            .concatArray(new Uint8Array(new Float32Array([this.Value]).buffer))
            .concatArray(new Uint8Array(new Float32Array([this.ValueDot]).buffer));
    }
}

class FloatInputService_AnalogPolynomialConfig extends Config {
    constructor(Degree){
        super();
        this.A = new Float32Array(Degree+1)
    }

    GetByteArray() {
        return new Uint8Array([ 
            2 
            ]).concatArray(new Uint8Array(this.A.buffer))
            .concatArray(new Uint8Array(new Float32Array([this.MinValue]).buffer))
            .concatArray(new Uint8Array(new Float32Array([this.MaxValue]).buffer))
            .concatArray(new Uint8Array(new Uint16Array([this.DotSampleRate]).buffer));
    }
}