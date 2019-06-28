#!/bin/bash -e

bestand=`ssh user@xxx.xxx.xxx.xxx ". .profile; sqlplus -S xxx/xxx@xxx <<EOF
set heading off
select sum(fc1d_k001)
  from DWH_DM.FAC_CONTR_G01_DAY x
  join dwh_dm.dim_contract_main c on c.dcma_id = x.fc1d_dcma_id
  join dwh_dm.dim_product p on c.dcma_dpro_id = p.dpro_id
  join dwh_dm.dim_product_service_type t on p.dpro_dpst_id = t.dpst_id
 where t.dpst_aax_id = 180
   and fc1d_dtim_id = (select max(fc1d_dtim_id) from DWH_DM.FAC_CONTR_G01_DAY)
;
exit
EOF"`
for f in ${bestand}
do
        if [ "$f" != "" ]; then post=${f}; fi
done
curl -d a=0 http://xxx.xxx.xxx.xxx:3000/?bestand=${post}
#echo ${bestand}

