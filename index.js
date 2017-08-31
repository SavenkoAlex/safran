const morpho = require('./bin/safran');
var status=-1;
var Safran = new morpho();

async function scanning() 
{
    if (status!=-1)
    {
        await Safran.cDev()
    }
    else 
    {
        status = await Safran.oDev();
        if (status==0)
        {
            await Safran.rFinger();
        }
        status=await Safran.cDev();
        return status;
    }
}

scanning().then (v=>{console.log("status :" + status)});



