const  safran = require ('../node_modules/dev/build/Release/openDev.node');

module.exports = class 
{
    constructor()
    {
        this.status =  -1;
    }

    oDev()
    {
        this.status=safran.getDev();
        return this.status;
    }

    cDev()
    {
        this.status=safran.closeDev();
        return this.status;
    }

    rFinger()
    {
        safran.getFinger();
    }
}