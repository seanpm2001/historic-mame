#include "driver.h"


#ifdef TINY_COMPILE
extern struct GameDriver TINY_NAME;

const struct GameDriver *drivers[] =
{
	&TINY_NAME,
	0	/* end of array */
};

#else

#ifndef NEOMAME

/* "Pacman hardware" games */
extern struct GameDriver pacman_driver;
extern struct GameDriver pacmanjp_driver;
extern struct GameDriver pacmanm_driver;
extern struct GameDriver npacmod_driver;
extern struct GameDriver pacmod_driver;
extern struct GameDriver hangly_driver;
extern struct GameDriver hangly2_driver;
extern struct GameDriver puckman_driver;
extern struct GameDriver pacheart_driver;
extern struct GameDriver piranha_driver;
extern struct GameDriver pacplus_driver;
extern struct GameDriver mspacman_driver;
extern struct GameDriver mspacatk_driver;
extern struct GameDriver pacgal_driver;
extern struct GameDriver maketrax_driver;
extern struct GameDriver crush_driver;
extern struct GameDriver crush2_driver;
extern struct GameDriver crush3_driver;
extern struct GameDriver mbrush_driver;
extern struct GameDriver eyes_driver;
extern struct GameDriver mrtnt_driver;
extern struct GameDriver ponpoko_driver;
extern struct GameDriver lizwiz_driver;
extern struct GameDriver theglob_driver;
extern struct GameDriver beastf_driver;
extern struct GameDriver jumpshot_driver;
extern struct GameDriver vanvan_driver;
extern struct GameDriver vanvanb_driver;
extern struct GameDriver alibaba_driver;
extern struct GameDriver pengo_driver;
extern struct GameDriver pengo2_driver;
extern struct GameDriver pengo2u_driver;
extern struct GameDriver penta_driver;
extern struct GameDriver jrpacman_driver;

/* "Galaxian hardware" games */
extern struct GameDriver galaxian_driver;
extern struct GameDriver galmidw_driver;
extern struct GameDriver superg_driver;
extern struct GameDriver galaxb_driver;
extern struct GameDriver galapx_driver;
extern struct GameDriver galap1_driver;
extern struct GameDriver galap4_driver;
extern struct GameDriver galturbo_driver;
extern struct GameDriver pisces_driver;
extern struct GameDriver japirem_driver;
extern struct GameDriver uniwars_driver;
extern struct GameDriver spacbatt_driver;
extern struct GameDriver warofbug_driver;
extern struct GameDriver redufo_driver;
extern struct GameDriver pacmanbl_driver;
extern struct GameDriver devilfsh_driver;
extern struct GameDriver zigzag_driver;
extern struct GameDriver zigzag2_driver;
extern struct GameDriver jumpbug_driver;
extern struct GameDriver jumpbugb_driver;
extern struct GameDriver mooncrgx_driver;
extern struct GameDriver mooncrst_driver;
extern struct GameDriver mooncrsg_driver;
extern struct GameDriver mooncrsb_driver;
extern struct GameDriver mooncrs2_driver;
extern struct GameDriver fantazia_driver;
extern struct GameDriver eagle_driver;
extern struct GameDriver eagle2_driver;
extern struct GameDriver moonqsr_driver;
extern struct GameDriver checkman_driver;
extern struct GameDriver moonal2_driver;
extern struct GameDriver moonal2b_driver;
extern struct GameDriver kingball_driver;
extern struct GameDriver kingbalj_driver;

/* "Scramble hardware" (and variations) games */
extern struct GameDriver scramble_driver;
extern struct GameDriver scrambls_driver;
extern struct GameDriver scramblb_driver;
extern struct GameDriver atlantis_driver;
extern struct GameDriver theend_driver;
extern struct GameDriver ckongs_driver;
extern struct GameDriver froggers_driver;
extern struct GameDriver amidars_driver;
extern struct GameDriver triplep_driver;
extern struct GameDriver mariner_driver;
extern struct GameDriver scobra_driver;
extern struct GameDriver scobras_driver;
extern struct GameDriver scobrab_driver;
extern struct GameDriver stratgyx_driver;
extern struct GameDriver stratgyb_driver;
extern struct GameDriver armorcar_driver;
extern struct GameDriver armorca2_driver;
extern struct GameDriver moonwar2_driver;
extern struct GameDriver monwar2a_driver;
extern struct GameDriver spdcoin_driver;
extern struct GameDriver darkplnt_driver;
extern struct GameDriver tazmania_driver;
extern struct GameDriver tazmani2_driver;
extern struct GameDriver calipso_driver;
extern struct GameDriver anteater_driver;
extern struct GameDriver rescue_driver;
extern struct GameDriver minefld_driver;
extern struct GameDriver losttomb_driver;
extern struct GameDriver losttmbh_driver;
extern struct GameDriver superbon_driver;
extern struct GameDriver hustler_driver;
extern struct GameDriver pool_driver;
extern struct GameDriver billiard_driver;
extern struct GameDriver frogger_driver;
extern struct GameDriver frogsega_driver;
extern struct GameDriver frogger2_driver;
extern struct GameDriver amidar_driver;
extern struct GameDriver amidaru_driver;
extern struct GameDriver amidaro_driver;
extern struct GameDriver amigo_driver;
extern struct GameDriver turtles_driver;
extern struct GameDriver turpin_driver;
extern struct GameDriver k600_driver;
extern struct GameDriver flyboy_driver;
extern struct GameDriver fastfred_driver;
extern struct GameDriver jumpcoas_driver;

/* "Crazy Climber hardware" games */
extern struct GameDriver cclimber_driver;
extern struct GameDriver ccjap_driver;
extern struct GameDriver ccboot_driver;
extern struct GameDriver ccboot2_driver;
extern struct GameDriver ckong_driver;
extern struct GameDriver ckonga_driver;
extern struct GameDriver ckongjeu_driver;
extern struct GameDriver ckongo_driver;
extern struct GameDriver ckongalc_driver;
extern struct GameDriver monkeyd_driver;
extern struct GameDriver rpatrolb_driver;
extern struct GameDriver silvland_driver;
extern struct GameDriver yamato_driver;
extern struct GameDriver yamato2_driver;
extern struct GameDriver swimmer_driver;
extern struct GameDriver swimmera_driver;
extern struct GameDriver guzzler_driver;

/* Nichibutsu games */
extern struct GameDriver friskyt_driver;
extern struct GameDriver radrad_driver;
extern struct GameDriver seicross_driver;
extern struct GameDriver sectrzon_driver;
extern struct GameDriver wiping_driver;
extern struct GameDriver rugrats_driver;
extern struct GameDriver cop01_driver;
extern struct GameDriver cop01a_driver;
extern struct GameDriver terracre_driver;
extern struct GameDriver terracra_driver;
extern struct GameDriver galivan_driver;
extern struct GameDriver galivan2_driver;
extern struct GameDriver dangar_driver;
extern struct GameDriver dangar2_driver;
extern struct GameDriver dangarb_driver;
extern struct GameDriver terraf_driver;
extern struct GameDriver terrafu_driver;
extern struct GameDriver armedf_driver;

/* "Phoenix hardware" (and variations) games */
extern struct GameDriver phoenix_driver;
extern struct GameDriver phoenixt_driver;
extern struct GameDriver phoenix3_driver;
extern struct GameDriver phoenixc_driver;
extern struct GameDriver pleiads_driver;
extern struct GameDriver pleiadce_driver;
extern struct GameDriver naughtyb_driver;
extern struct GameDriver naughtya_driver;
extern struct GameDriver naughtyc_driver;
extern struct GameDriver popflame_driver;
extern struct GameDriver popflama_driver;

/* Namco games */
extern struct GameDriver warpwarp_driver;
extern struct GameDriver warpwarr_driver;
extern struct GameDriver warpwar2_driver;
extern struct GameDriver rallyx_driver;
extern struct GameDriver rallyxm_driver;
extern struct GameDriver nrallyx_driver;
extern struct GameDriver jungler_driver;
extern struct GameDriver junglers_driver;
extern struct GameDriver locomotn_driver;
extern struct GameDriver commsega_driver;
extern struct GameDriver bosco_driver;
extern struct GameDriver boscomd_driver;
extern struct GameDriver boscomd2_driver;
extern struct GameDriver galaga_driver;
extern struct GameDriver galagamw_driver;
extern struct GameDriver galagads_driver;
extern struct GameDriver gallag_driver;
extern struct GameDriver galagab2_driver;
extern struct GameDriver galaga84_driver;
extern struct GameDriver digdug_driver;
extern struct GameDriver digdugnm_driver;
extern struct GameDriver dzigzag_driver;
extern struct GameDriver xevious_driver;
extern struct GameDriver xeviousa_driver;
extern struct GameDriver xevios_driver;
extern struct GameDriver sxevious_driver;
extern struct GameDriver superpac_driver;
extern struct GameDriver superpcn_driver;
extern struct GameDriver pacnpal_driver;
extern struct GameDriver phozon_driver;
extern struct GameDriver mappy_driver;
extern struct GameDriver mappyjp_driver;
extern struct GameDriver digdug2_driver;
extern struct GameDriver todruaga_driver;
extern struct GameDriver todruagb_driver;
extern struct GameDriver motos_driver;
extern struct GameDriver grobda_driver;
extern struct GameDriver grobda2_driver;
extern struct GameDriver grobda3_driver;
extern struct GameDriver gaplus_driver;
extern struct GameDriver gaplusa_driver;
extern struct GameDriver galaga3_driver;
extern struct GameDriver galaga3a_driver;
extern struct GameDriver pacland_driver;
extern struct GameDriver pacland2_driver;
extern struct GameDriver pacland3_driver;
extern struct GameDriver paclandm_driver;

/* Namco System 86 games */
extern struct GameDriver roishtar_driver;
extern struct GameDriver genpeitd_driver;
extern struct GameDriver rthunder_driver;
extern struct GameDriver rthundrb_driver;
extern struct GameDriver wndrmomo_driver;

/* Namco System 1 games */
extern struct GameDriver blazer_driver;
extern struct GameDriver ws90_driver;
extern struct GameDriver dspirits_driver;
extern struct GameDriver splatter_driver;
extern struct GameDriver galaga88_driver;
extern struct GameDriver pacmania_driver;
extern struct GameDriver alice_driver;
extern struct GameDriver shadowld_driver;

/* Namco System 2 games */
extern struct GameDriver cosmogng_driver;
extern struct GameDriver assault_driver;
extern struct GameDriver phelios_driver;
extern struct GameDriver rthun2_driver;
extern struct GameDriver metlhawk_driver;

/* Universal games */
extern struct GameDriver cosmicg_driver;
extern struct GameDriver cosmica_driver;
extern struct GameDriver panic_driver;
extern struct GameDriver panica_driver;
extern struct GameDriver panicger_driver;
extern struct GameDriver magspot2_driver;
extern struct GameDriver devzone_driver;
extern struct GameDriver nomnlnd_driver;
extern struct GameDriver nomnlndg_driver;
extern struct GameDriver cheekyms_driver;
extern struct GameDriver ladybug_driver;
extern struct GameDriver ladybugb_driver;
extern struct GameDriver snapjack_driver;
extern struct GameDriver cavenger_driver;
extern struct GameDriver mrdo_driver;
extern struct GameDriver mrdot_driver;
extern struct GameDriver mrdofix_driver;
extern struct GameDriver mrlo_driver;
extern struct GameDriver mrdu_driver;
extern struct GameDriver mrdoy_driver;
extern struct GameDriver yankeedo_driver;
extern struct GameDriver docastle_driver;
extern struct GameDriver docastl2_driver;
extern struct GameDriver dounicorn_driver;
extern struct GameDriver dorunrun_driver;
extern struct GameDriver dorunru2_driver;
extern struct GameDriver dorunruc_driver;
extern struct GameDriver spiero_driver;
extern struct GameDriver dowild_driver;
extern struct GameDriver jjack_driver;
extern struct GameDriver kickridr_driver;

/* Nintendo games */
extern struct GameDriver radarscp_driver;
extern struct GameDriver dkong_driver;
extern struct GameDriver dkongjp_driver;
extern struct GameDriver dkongjr_driver;
extern struct GameDriver dkngjrjp_driver;
extern struct GameDriver dkjrjp_driver;
extern struct GameDriver dkjrbl_driver;
extern struct GameDriver dkong3_driver;
extern struct GameDriver mario_driver;
extern struct GameDriver masao_driver;
extern struct GameDriver hunchy_driver;
extern struct GameDriver herocast_driver;
extern struct GameDriver popeye_driver;
extern struct GameDriver popeye2_driver;
extern struct GameDriver popeyebl_driver;
extern struct GameDriver punchout_driver;
extern struct GameDriver spnchout_driver;
extern struct GameDriver armwrest_driver;

/* Midway 8080 b/w games */
extern struct GameDriver seawolf_driver;
extern struct GameDriver gunfight_driver;
extern struct GameDriver tornbase_driver;
extern struct GameDriver zzzap_driver;
extern struct GameDriver maze_driver;
extern struct GameDriver boothill_driver;
extern struct GameDriver checkmat_driver;
extern struct GameDriver gmissile_driver;
extern struct GameDriver clowns_driver;
extern struct GameDriver spcenctr_driver;
extern struct GameDriver invaders_driver;
extern struct GameDriver invdelux_driver;
extern struct GameDriver invadpt2_driver;
extern struct GameDriver earthinv_driver;
extern struct GameDriver spaceatt_driver;
extern struct GameDriver sinvemag_driver;
extern struct GameDriver invrvnge_driver;
extern struct GameDriver invrvnga_driver;
extern struct GameDriver galxwars_driver;
extern struct GameDriver lrescue_driver;
extern struct GameDriver grescue_driver;
extern struct GameDriver desterth_driver;
extern struct GameDriver cosmicmo_driver;
extern struct GameDriver rollingc_driver;
extern struct GameDriver bandido_driver;
extern struct GameDriver ozmawars_driver;
extern struct GameDriver solfight_driver;
extern struct GameDriver spaceph_driver;
extern struct GameDriver schaser_driver;
extern struct GameDriver lupin3_driver;
extern struct GameDriver helifire_driver;
extern struct GameDriver helifira_driver;
extern struct GameDriver spacefev_driver;
extern struct GameDriver sfeverbw_driver;
extern struct GameDriver astlaser_driver;
extern struct GameDriver intruder_driver;
extern struct GameDriver polaris_driver;
extern struct GameDriver polarisa_driver;
extern struct GameDriver ballbomb_driver;
extern struct GameDriver lagunar_driver;
extern struct GameDriver m4_driver;
extern struct GameDriver phantom2_driver;
extern struct GameDriver dogpatch_driver;
extern struct GameDriver midwbowl_driver;
extern struct GameDriver blueshrk_driver;
extern struct GameDriver einnings_driver;
extern struct GameDriver dplay_driver;
extern struct GameDriver m79amb_driver;
extern struct GameDriver alieninv2_driver;
extern struct GameDriver sitv_driver;
extern struct GameDriver sicv_driver;
extern struct GameDriver sisv_driver;

/* "Midway" Z80 b/w games */
extern struct GameDriver astinvad_driver;
extern struct GameDriver kamikaze_driver;
extern struct GameDriver spaceint_driver;

/* Meadows S2650 games */
extern struct GameDriver lazercmd_driver;
extern struct GameDriver deadeye_driver;
extern struct GameDriver gypsyjug_driver;
extern struct GameDriver medlanes_driver;

/* Midway "Astrocade" games */
extern struct GameDriver wow_driver;
extern struct GameDriver robby_driver;
extern struct GameDriver gorf_driver;
extern struct GameDriver gorfpgm1_driver;
extern struct GameDriver seawolf2_driver;
extern struct GameDriver spacezap_driver;
extern struct GameDriver ebases_driver;

/* Bally Midway MCR games */
extern struct GameDriver solarfox_driver;
extern struct GameDriver kick_driver;
extern struct GameDriver kicka_driver;
extern struct GameDriver shollow_driver;
extern struct GameDriver shollow2_driver;
extern struct GameDriver tron_driver;
extern struct GameDriver tron2_driver;
extern struct GameDriver kroozr_driver;
extern struct GameDriver domino_driver;
extern struct GameDriver wacko_driver;
extern struct GameDriver twotiger_driver;
extern struct GameDriver journey_driver;
extern struct GameDriver tapper_driver;
extern struct GameDriver sutapper_driver;
extern struct GameDriver rbtapper_driver;
extern struct GameDriver dotron_driver;
extern struct GameDriver dotrone_driver;
extern struct GameDriver destderb_driver;
extern struct GameDriver timber_driver;
extern struct GameDriver spyhunt_driver;
extern struct GameDriver crater_driver;
extern struct GameDriver sarge_driver;
extern struct GameDriver rampage_driver;
extern struct GameDriver rampage2_driver;
extern struct GameDriver powerdrv_driver;
extern struct GameDriver maxrpm_driver;
extern struct GameDriver xenophob_driver;

/* Irem games */
extern struct GameDriver skychut_driver;
extern struct GameDriver mpatrol_driver;
extern struct GameDriver mpatrolw_driver;
extern struct GameDriver mranger_driver;
extern struct GameDriver troangel_driver;
extern struct GameDriver yard_driver;
extern struct GameDriver vsyard_driver;
extern struct GameDriver vsyard2_driver;
extern struct GameDriver travrusa_driver;
extern struct GameDriver motorace_driver;
extern struct GameDriver kungfum_driver;
extern struct GameDriver kungfud_driver;
extern struct GameDriver kungfub_driver;
extern struct GameDriver kungfub2_driver;
extern struct GameDriver battroad_driver;
extern struct GameDriver ldrun_driver;
extern struct GameDriver ldruna_driver;
extern struct GameDriver ldrun2_driver;
extern struct GameDriver ldrun3_driver;
extern struct GameDriver ldrun4_driver;
extern struct GameDriver lotlot_driver;
extern struct GameDriver kidniki_driver;
extern struct GameDriver spelunk2_driver;
extern struct GameDriver vigilant_driver;
extern struct GameDriver vigilntu_driver;
extern struct GameDriver vigilntj_driver;

/* Gottlieb/Mylstar games (Gottlieb became Mylstar in 1983) */
extern struct GameDriver reactor_driver;
extern struct GameDriver mplanets_driver;
extern struct GameDriver qbert_driver;
extern struct GameDriver qbertjp_driver;
extern struct GameDriver sqbert_driver;
extern struct GameDriver krull_driver;
extern struct GameDriver mach3_driver;
extern struct GameDriver usvsthem_driver;
extern struct GameDriver stooges_driver;
extern struct GameDriver qbertqub_driver;
extern struct GameDriver curvebal_driver;

/* older Taito games */
extern struct GameDriver crbaloon_driver;
extern struct GameDriver crbalon2_driver;

/* Taito "Qix hardware" games */
extern struct GameDriver qix_driver;
extern struct GameDriver qixa_driver;
extern struct GameDriver qix2_driver;
extern struct GameDriver sdungeon_driver;
extern struct GameDriver elecyoyo_driver;
extern struct GameDriver elecyoy2_driver;
extern struct GameDriver kram_driver;
extern struct GameDriver kram2_driver;
extern struct GameDriver zookeep_driver;
extern struct GameDriver zookeep2_driver;
extern struct GameDriver zookeep3_driver;

/* Taito SJ System games */
extern struct GameDriver spaceskr_driver;
extern struct GameDriver junglek_driver;
extern struct GameDriver jhunt_driver;
extern struct GameDriver alpine_driver;
extern struct GameDriver alpinea_driver;
extern struct GameDriver timetunl_driver;
extern struct GameDriver wwestern_driver;
extern struct GameDriver frontlin_driver;
extern struct GameDriver elevator_driver;
extern struct GameDriver elevatob_driver;
extern struct GameDriver tinstar_driver;
extern struct GameDriver waterski_driver;
extern struct GameDriver bioatack_driver;
extern struct GameDriver sfposeid_driver;

/* other Taito games */
extern struct GameDriver bking2_driver;
extern struct GameDriver gsword_driver;
extern struct GameDriver retofinv_driver;
extern struct GameDriver retofin1_driver;
extern struct GameDriver retofin2_driver;
extern struct GameDriver gladiatr_driver;
extern struct GameDriver ogonsiro_driver;
extern struct GameDriver gcastle_driver;
extern struct GameDriver bublbobl_driver;
extern struct GameDriver bublbobr_driver;
extern struct GameDriver boblbobl_driver;
extern struct GameDriver sboblbob_driver;
extern struct GameDriver tokio_driver;
extern struct GameDriver tokiob_driver;
extern struct GameDriver mexico86_driver;
extern struct GameDriver kicknrun_driver;
extern struct GameDriver rastan_driver;
extern struct GameDriver rastanu_driver;
extern struct GameDriver rastanu2_driver;
extern struct GameDriver rastsaga_driver;
extern struct GameDriver rastsag2_driver;
extern struct GameDriver rainbow_driver;
extern struct GameDriver rainbowe_driver;
extern struct GameDriver jumping_driver;
extern struct GameDriver arkanoid_driver;
extern struct GameDriver arknoidu_driver;
extern struct GameDriver arkbl2_driver;
extern struct GameDriver arkatayt_driver;
extern struct GameDriver superqix_driver;
extern struct GameDriver sqixbl_driver;
extern struct GameDriver twincobr_driver;
extern struct GameDriver twincobu_driver;
extern struct GameDriver ktiger_driver;
extern struct GameDriver extrmatn_driver;
extern struct GameDriver arkanoi2_driver;
extern struct GameDriver ark2us_driver;
extern struct GameDriver tnzs_driver;
extern struct GameDriver tnzsb_driver;
extern struct GameDriver tnzs2_driver;
extern struct GameDriver insectx_driver;
extern struct GameDriver tigerh_driver;
extern struct GameDriver tigerh2_driver;
extern struct GameDriver tigerhb1_driver;
extern struct GameDriver tigerhb2_driver;
extern struct GameDriver slapfigh_driver;
extern struct GameDriver slapbtjp_driver;
extern struct GameDriver slapbtuk_driver;
extern struct GameDriver getstar_driver;
extern struct GameDriver superman_driver;
extern struct GameDriver lkage_driver;

/* Taito F2 games */
extern struct GameDriver ssi_driver;
extern struct GameDriver majest12_driver;
extern struct GameDriver liquidk_driver;
extern struct GameDriver growl_driver;

/* Toaplan games */
extern struct GameDriver truxton_driver;
extern struct GameDriver zerowing_driver;
extern struct GameDriver hellfire_driver;
extern struct GameDriver outzone_driver;
extern struct GameDriver vimana_driver;
extern struct GameDriver vimana2_driver;
extern struct GameDriver snowbros_driver;
extern struct GameDriver snowbroa_driver;

/* Williams games */
extern struct GameDriver robotron_driver;
extern struct GameDriver robotryo_driver;
extern struct GameDriver stargate_driver;
extern struct GameDriver joust_driver;
extern struct GameDriver joustwr_driver;
extern struct GameDriver joustr_driver;
extern struct GameDriver sinistar_driver;
extern struct GameDriver sinista1_driver;
extern struct GameDriver sinista2_driver;
extern struct GameDriver bubbles_driver;
extern struct GameDriver bubblesr_driver;
extern struct GameDriver defender_driver;
extern struct GameDriver defendg_driver;
extern struct GameDriver splat_driver;
extern struct GameDriver blaster_driver;
extern struct GameDriver colony7_driver;
extern struct GameDriver colony7a_driver;
extern struct GameDriver lottofun_driver;
extern struct GameDriver defcmnd_driver;
extern struct GameDriver defence_driver;
extern struct GameDriver mysticm_driver;
extern struct GameDriver tshoot_driver;
extern struct GameDriver inferno_driver;
extern struct GameDriver joust2_driver;

/* Capcom games */
extern struct GameDriver vulgus_driver;
extern struct GameDriver vulgusj_driver;
extern struct GameDriver sonson_driver;
extern struct GameDriver higemaru_driver;
extern struct GameDriver c1942_driver;
extern struct GameDriver c1942a_driver;
extern struct GameDriver c1942b_driver;
extern struct GameDriver exedexes_driver;
extern struct GameDriver savgbees_driver;
extern struct GameDriver commando_driver;
extern struct GameDriver commandu_driver;
extern struct GameDriver commandj_driver;
extern struct GameDriver spaceinv_driver;
extern struct GameDriver gng_driver;
extern struct GameDriver gngt_driver;
extern struct GameDriver gngcross_driver;
extern struct GameDriver gngjap_driver;
extern struct GameDriver diamond_driver;
extern struct GameDriver gunsmoke_driver;
extern struct GameDriver gunsmrom_driver;
extern struct GameDriver gunsmoka_driver;
extern struct GameDriver gunsmokj_driver;
extern struct GameDriver sectionz_driver;
extern struct GameDriver trojan_driver;
extern struct GameDriver trojanj_driver;
extern struct GameDriver srumbler_driver;
extern struct GameDriver srumblr2_driver;
extern struct GameDriver lwings_driver;
extern struct GameDriver lwings2_driver;
extern struct GameDriver lwingsjp_driver;
extern struct GameDriver sidearms_driver;
extern struct GameDriver sidearmr_driver;
extern struct GameDriver sidearjp_driver;
extern struct GameDriver turtship_driver;
extern struct GameDriver dyger_driver;
extern struct GameDriver avengers_driver;
extern struct GameDriver avenger2_driver;
extern struct GameDriver bionicc_driver;
extern struct GameDriver bionicc2_driver;
extern struct GameDriver topsecrt_driver;
extern struct GameDriver c1943_driver;
extern struct GameDriver c1943jap_driver;
extern struct GameDriver blktiger_driver;
extern struct GameDriver bktigerb_driver;
extern struct GameDriver blkdrgon_driver;
extern struct GameDriver blkdrgnb_driver;
extern struct GameDriver sf1_driver;
extern struct GameDriver sf1us_driver;
extern struct GameDriver sf1jp_driver;
extern struct GameDriver tigeroad_driver;
extern struct GameDriver f1dream_driver;
extern struct GameDriver f1dreamb_driver;
extern struct GameDriver c1943kai_driver;
extern struct GameDriver lastduel_driver;
extern struct GameDriver lstduela_driver;
extern struct GameDriver lstduelb_driver;
extern struct GameDriver forgottn_driver;
extern struct GameDriver ghouls_driver;
extern struct GameDriver ghoulsj_driver;
extern struct GameDriver madgear_driver;
extern struct GameDriver ledstorm_driver;
extern struct GameDriver strider_driver;
extern struct GameDriver striderj_driver;
extern struct GameDriver dwj_driver;
extern struct GameDriver willow_driver;
extern struct GameDriver willowj_driver;
extern struct GameDriver unsquad_driver;
extern struct GameDriver area88_driver;
extern struct GameDriver ffight_driver;
extern struct GameDriver ffightu_driver;
extern struct GameDriver ffightj_driver;
extern struct GameDriver c1941_driver;
extern struct GameDriver c1941j_driver;
extern struct GameDriver mercs_driver;
extern struct GameDriver mercsu_driver;
extern struct GameDriver mercsj_driver;
extern struct GameDriver mtwins_driver;
extern struct GameDriver chikij_driver;
extern struct GameDriver msword_driver;
extern struct GameDriver mswordu_driver;
extern struct GameDriver mswordj_driver;
extern struct GameDriver cawing_driver;
extern struct GameDriver cawingj_driver;
extern struct GameDriver nemo_driver;
extern struct GameDriver nemoj_driver;
extern struct GameDriver sf2_driver;
extern struct GameDriver sf2a_driver;
extern struct GameDriver sf2b_driver;
extern struct GameDriver sf2e_driver;
extern struct GameDriver sf2j_driver;
extern struct GameDriver c3wonders_driver;
extern struct GameDriver c3wonderj_driver;
extern struct GameDriver kod_driver;
extern struct GameDriver kodj_driver;
extern struct GameDriver kodb_driver;
extern struct GameDriver captcomm_driver;
extern struct GameDriver captcomu_driver;
extern struct GameDriver captcomj_driver;
extern struct GameDriver knights_driver;
extern struct GameDriver knightsj_driver;
extern struct GameDriver sf2ce_driver;
extern struct GameDriver sf2cea_driver;
extern struct GameDriver sf2ceb_driver;
extern struct GameDriver sf2cej_driver;
extern struct GameDriver sf2red_driver;
extern struct GameDriver sf2rb_driver;
extern struct GameDriver varth_driver;
extern struct GameDriver varthj_driver;
extern struct GameDriver cworld2j_driver;
extern struct GameDriver wof_driver;
extern struct GameDriver wofj_driver;
extern struct GameDriver sf2t_driver;
extern struct GameDriver sf2tj_driver;
extern struct GameDriver dino_driver;
extern struct GameDriver punisher_driver;
extern struct GameDriver punishrj_driver;
extern struct GameDriver slammast_driver;
extern struct GameDriver mbomber_driver;
extern struct GameDriver mbomberj_driver;
extern struct GameDriver pnickj_driver;
extern struct GameDriver qad_driver;
extern struct GameDriver qadj_driver;
extern struct GameDriver megaman_driver;
extern struct GameDriver rockmanj_driver;

/* "Capcom Bowling hardware" games */
extern struct GameDriver capbowl_driver;
extern struct GameDriver capbowl2_driver;
extern struct GameDriver clbowl_driver;
extern struct GameDriver bowlrama_driver;

/* Mitchell games */
extern struct GameDriver pang_driver;
extern struct GameDriver pangb_driver;
extern struct GameDriver bbros_driver;
extern struct GameDriver hatena_driver;
extern struct GameDriver spang_driver;
extern struct GameDriver sbbros_driver;
extern struct GameDriver qtono1_driver;
extern struct GameDriver block_driver;
extern struct GameDriver blockbl_driver;

/* Gremlin 8080 games */
extern struct GameDriver blockade_driver;
extern struct GameDriver comotion_driver;
extern struct GameDriver hustle_driver;
extern struct GameDriver blasto_driver;

/* Gremlin/Sega "VIC dual game board" games */
extern struct GameDriver depthch_driver;
extern struct GameDriver safari_driver;
extern struct GameDriver frogs_driver;
extern struct GameDriver sspaceat_driver;
extern struct GameDriver sspacatc_driver;
extern struct GameDriver headon_driver;
extern struct GameDriver headonb_driver;
extern struct GameDriver headon2_driver;
extern struct GameDriver invho2_driver;
extern struct GameDriver samurai_driver;
extern struct GameDriver invinco_driver;
extern struct GameDriver invds_driver;
extern struct GameDriver tranqgun_driver;
extern struct GameDriver spacetrk_driver;
extern struct GameDriver sptrekct_driver;
extern struct GameDriver carnival_driver;
extern struct GameDriver carnvckt_driver;
extern struct GameDriver digger_driver;
extern struct GameDriver pulsar_driver;
extern struct GameDriver heiankyo_driver;

/* Sega G-80 vector games */
extern struct GameDriver spacfury_driver;
extern struct GameDriver spacfura_driver;
extern struct GameDriver zektor_driver;
extern struct GameDriver tacscan_driver;
extern struct GameDriver elim2_driver;
extern struct GameDriver elim2a_driver;
extern struct GameDriver elim4_driver;
extern struct GameDriver startrek_driver;

/* Sega G-80 raster games */
extern struct GameDriver astrob_driver;
extern struct GameDriver astrob1_driver;
extern struct GameDriver s005_driver;
extern struct GameDriver monsterb_driver;
extern struct GameDriver spaceod_driver;
extern struct GameDriver pignewt_driver;
extern struct GameDriver pignewta_driver;
extern struct GameDriver sindbadm_driver;

/* Sega "Zaxxon hardware" games */
extern struct GameDriver zaxxon_driver;
extern struct GameDriver zaxxon2_driver;
extern struct GameDriver szaxxon_driver;
extern struct GameDriver futspy_driver;
extern struct GameDriver congo_driver;
extern struct GameDriver tiptop_driver;

/* Sega System 1 / System 2 games */
extern struct GameDriver starjack_driver;
extern struct GameDriver starjacs_driver;
extern struct GameDriver regulus_driver;
extern struct GameDriver regulusu_driver;
extern struct GameDriver upndown_driver;
extern struct GameDriver mrviking_driver;
extern struct GameDriver swat_driver;
extern struct GameDriver flicky_driver;
extern struct GameDriver flicky2_driver;
extern struct GameDriver bullfgtj_driver;
extern struct GameDriver pitfall2_driver;
extern struct GameDriver pitfallu_driver;
extern struct GameDriver seganinj_driver;
extern struct GameDriver seganinu_driver;
extern struct GameDriver nprinces_driver;
extern struct GameDriver nprincsu_driver;
extern struct GameDriver nprincsb_driver;
extern struct GameDriver imsorry_driver;
extern struct GameDriver imsorryj_driver;
extern struct GameDriver teddybb_driver;
extern struct GameDriver hvymetal_driver;
extern struct GameDriver myhero_driver;
extern struct GameDriver myheroj_driver;
extern struct GameDriver myherok_driver;
extern struct GameDriver shtngmst_driver;
extern struct GameDriver chplft_driver;
extern struct GameDriver chplftb_driver;
extern struct GameDriver chplftbl_driver;
extern struct GameDriver fdwarrio_driver;
extern struct GameDriver brain_driver;
extern struct GameDriver wboy_driver;
extern struct GameDriver wboy2_driver;
extern struct GameDriver wboy3_driver;
extern struct GameDriver wboy4_driver;
extern struct GameDriver wboyu_driver;
extern struct GameDriver wboy4u_driver;
extern struct GameDriver wbdeluxe_driver;
extern struct GameDriver gardia_driver;
extern struct GameDriver blockgal_driver;
extern struct GameDriver tokisens_driver;
extern struct GameDriver dakkochn_driver;
extern struct GameDriver ufosensi_driver;
extern struct GameDriver wbml_driver;

/* other Sega 8-bit games */
extern struct GameDriver turbo_driver;
extern struct GameDriver turboa_driver;
extern struct GameDriver turbob_driver;
extern struct GameDriver kopunch_driver;
extern struct GameDriver suprloco_driver;
extern struct GameDriver champbas_driver;
extern struct GameDriver champbb2_driver;
extern struct GameDriver bankp_driver;

/* Sega System 16 games */
extern struct GameDriver alexkidd_driver;
extern struct GameDriver aliensyn_driver;
extern struct GameDriver altbeast_driver;
extern struct GameDriver astormbl_driver;
extern struct GameDriver aurail_driver;
extern struct GameDriver dduxbl_driver;
extern struct GameDriver eswatbl_driver;
extern struct GameDriver fantzone_driver;
extern struct GameDriver fpointbl_driver;
extern struct GameDriver goldnaxe_driver;
extern struct GameDriver hwchamp_driver;
extern struct GameDriver mjleague_driver;
extern struct GameDriver passshtb_driver;
extern struct GameDriver quartet2_driver;
extern struct GameDriver sdi_driver;
extern struct GameDriver shinobi_driver;
extern struct GameDriver tetrisbl_driver;
extern struct GameDriver timscanr_driver;
extern struct GameDriver tturfbl_driver;
extern struct GameDriver wb3bl_driver;
extern struct GameDriver wrestwar_driver;

/* Data East "Burger Time hardware" games */
extern struct GameDriver lnc_driver;
extern struct GameDriver zoar_driver;
extern struct GameDriver btime_driver;
extern struct GameDriver btimed_driver;
extern struct GameDriver btimed2_driver;
extern struct GameDriver wtennis_driver;
extern struct GameDriver bnj_driver;
extern struct GameDriver brubber_driver;
extern struct GameDriver caractn_driver;
extern struct GameDriver disco_driver;
extern struct GameDriver mmonkey_driver;
extern struct GameDriver eggs_driver;
extern struct GameDriver scregg_driver;
extern struct GameDriver tagteam_driver;
/* cassette system */
extern struct GameDriver decocass_driver;
extern struct GameDriver cookrace_driver;

/* other Data East games */
extern struct GameDriver astrof_driver;
extern struct GameDriver astrof2_driver;
extern struct GameDriver astrof3_driver;
extern struct GameDriver tomahawk_driver;
extern struct GameDriver tomahaw5_driver;
extern struct GameDriver kchamp_driver;
extern struct GameDriver kchampvs_driver;
extern struct GameDriver karatedo_driver;
extern struct GameDriver firetrap_driver;
extern struct GameDriver firetpbl_driver;
extern struct GameDriver brkthru_driver;
extern struct GameDriver brkthruj_driver;
extern struct GameDriver darwin_driver;
extern struct GameDriver shootout_driver;
extern struct GameDriver sidepckt_driver;
extern struct GameDriver exprraid_driver;
extern struct GameDriver wexpress_driver;
extern struct GameDriver wexpresb_driver;
extern struct GameDriver pcktgal_driver;
extern struct GameDriver pcktgalb_driver;
extern struct GameDriver pcktgal2_driver;
extern struct GameDriver spool3_driver;
extern struct GameDriver spool3i_driver;
extern struct GameDriver actfancr_driver;

/* Data East 8-bit games */
extern struct GameDriver lastmiss_driver;
extern struct GameDriver lastmss2_driver;
extern struct GameDriver shackled_driver;
extern struct GameDriver breywood_driver;
extern struct GameDriver csilver_driver;
extern struct GameDriver ghostb_driver;
extern struct GameDriver ghostb3_driver;
extern struct GameDriver mazeh_driver;
extern struct GameDriver srdarwin_driver;
extern struct GameDriver gondo_driver;
extern struct GameDriver mekyosen_driver;
extern struct GameDriver cobracom_driver;
extern struct GameDriver oscar_driver;
extern struct GameDriver oscarj_driver;

/* Data East 16-bit games */
extern struct GameDriver karnov_driver;
extern struct GameDriver karnovj_driver;
extern struct GameDriver chelnov_driver;
extern struct GameDriver chelnovj_driver;
extern struct GameDriver hbarrel_driver;
extern struct GameDriver hbarrelj_driver;
extern struct GameDriver baddudes_driver;
extern struct GameDriver drgninja_driver;
extern struct GameDriver robocopp_driver;
extern struct GameDriver hippodrm_driver;
extern struct GameDriver ffantasy_driver;
extern struct GameDriver midres_driver;
extern struct GameDriver midresj_driver;
extern struct GameDriver slyspy_driver;
extern struct GameDriver slyspy2_driver;
extern struct GameDriver bouldash_driver;
extern struct GameDriver darkseal_driver;
extern struct GameDriver darksea1_driver;
extern struct GameDriver gatedoom_driver;
extern struct GameDriver gatedom1_driver;
extern struct GameDriver supbtime_driver;
extern struct GameDriver cninja_driver;
extern struct GameDriver cninja0_driver;
extern struct GameDriver cninjau_driver;
extern struct GameDriver joemac_driver;
extern struct GameDriver stoneage_driver;
extern struct GameDriver tumblep_driver;
extern struct GameDriver tumblep2_driver;

/* Tehkan / Tecmo games (Tehkan became Tecmo in 1986) */
extern struct GameDriver senjyo_driver;
extern struct GameDriver starforc_driver;
extern struct GameDriver starfore_driver;
extern struct GameDriver megaforc_driver;
extern struct GameDriver bombjack_driver;
extern struct GameDriver bombjac2_driver;
extern struct GameDriver pbaction_driver;
extern struct GameDriver pbactio2_driver;
extern struct GameDriver pontoon_driver;
extern struct GameDriver tehkanwc_driver;
extern struct GameDriver gridiron_driver;
extern struct GameDriver teedoff_driver;
extern struct GameDriver solomon_driver;
extern struct GameDriver rygar_driver;
extern struct GameDriver rygar2_driver;
extern struct GameDriver rygarj_driver;
extern struct GameDriver gemini_driver;
extern struct GameDriver silkworm_driver;
extern struct GameDriver silkwrm2_driver;
extern struct GameDriver gaiden_driver;
extern struct GameDriver shadoww_driver;
extern struct GameDriver tknight_driver;
extern struct GameDriver wildfang_driver;
extern struct GameDriver wc90_driver;
extern struct GameDriver wc90b_driver;

/* Konami bitmap games */
extern struct GameDriver tutankhm_driver;
extern struct GameDriver tutankst_driver;
extern struct GameDriver junofrst_driver;

/* Konami games */
extern struct GameDriver pooyan_driver;
extern struct GameDriver pooyans_driver;
extern struct GameDriver pootan_driver;
extern struct GameDriver timeplt_driver;
extern struct GameDriver timepltc_driver;
extern struct GameDriver spaceplt_driver;
extern struct GameDriver megazone_driver;
extern struct GameDriver rocnrope_driver;
extern struct GameDriver ropeman_driver;
extern struct GameDriver gyruss_driver;
extern struct GameDriver gyrussce_driver;
extern struct GameDriver venus_driver;
extern struct GameDriver trackfld_driver;
extern struct GameDriver trackflc_driver;
extern struct GameDriver hyprolym_driver;
extern struct GameDriver hyprolyb_driver;
extern struct GameDriver circusc_driver;
extern struct GameDriver circusc2_driver;
extern struct GameDriver circuscc_driver;
extern struct GameDriver tp84_driver;
extern struct GameDriver tp84a_driver;
extern struct GameDriver hyperspt_driver;
extern struct GameDriver sbasketb_driver;
extern struct GameDriver mikie_driver;
extern struct GameDriver mikiej_driver;
extern struct GameDriver mikiehs_driver;
extern struct GameDriver roadf_driver;
extern struct GameDriver roadf2_driver;
extern struct GameDriver yiear_driver;
extern struct GameDriver yiear2_driver;
extern struct GameDriver kicker_driver;
extern struct GameDriver shaolins_driver;
extern struct GameDriver pingpong_driver;
extern struct GameDriver gberet_driver;
extern struct GameDriver rushatck_driver;
extern struct GameDriver gberetb_driver;
extern struct GameDriver jailbrek_driver;
extern struct GameDriver ironhors_driver;
extern struct GameDriver farwest_driver;
extern struct GameDriver jackal_driver;
extern struct GameDriver topgunr_driver;
extern struct GameDriver topgunbl_driver;
extern struct GameDriver ddrible_driver;
extern struct GameDriver contra_driver;
extern struct GameDriver contrab_driver;
extern struct GameDriver gryzorb_driver;
extern struct GameDriver mainevt_driver;
extern struct GameDriver mainevt2_driver;
extern struct GameDriver devstors_driver;
extern struct GameDriver combasc_driver;
extern struct GameDriver combascb_driver;

/* Konami "Nemesis hardware" games */
extern struct GameDriver nemesis_driver;
extern struct GameDriver nemesuk_driver;
extern struct GameDriver konamigt_driver;
extern struct GameDriver salamand_driver;
extern struct GameDriver lifefrce_driver;
extern struct GameDriver lifefrcj_driver;
/* GX400 BIOS based games */
extern struct GameDriver rf2_driver;
extern struct GameDriver twinbee_driver;
extern struct GameDriver gradius_driver;
extern struct GameDriver gwarrior_driver;

/* Konami "TMNT hardware" games */
extern struct GameDriver tmnt_driver;
extern struct GameDriver tmntj_driver;
extern struct GameDriver tmht2p_driver;
extern struct GameDriver tmnt2pj_driver;
extern struct GameDriver punkshot_driver;
extern struct GameDriver punksht2_driver;

/* Exidy games */
extern struct GameDriver sidetrac_driver;
extern struct GameDriver targ_driver;
extern struct GameDriver spectar_driver;
extern struct GameDriver spectar1_driver;
extern struct GameDriver venture_driver;
extern struct GameDriver venture2_driver;
extern struct GameDriver venture4_driver;
extern struct GameDriver mtrap_driver;
extern struct GameDriver mtrap3_driver;
extern struct GameDriver mtrap4_driver;
extern struct GameDriver pepper2_driver;
extern struct GameDriver hardhat_driver;
extern struct GameDriver fax_driver;
extern struct GameDriver circus_driver;
extern struct GameDriver robotbwl_driver;
extern struct GameDriver crash_driver;
extern struct GameDriver ripcord_driver;
extern struct GameDriver starfire_driver;
extern struct GameDriver fireone_driver;

/* Exidy 440 games */
extern struct GameDriver crossbow_driver;
extern struct GameDriver cheyenne_driver;
extern struct GameDriver combat_driver;
extern struct GameDriver cracksht_driver;
extern struct GameDriver claypign_driver;
extern struct GameDriver chiller_driver;
extern struct GameDriver topsecex_driver;
extern struct GameDriver hitnmiss_driver;
extern struct GameDriver hitnmis2_driver;
extern struct GameDriver whodunit_driver;
extern struct GameDriver showdown_driver;

/* Atari vector games */
extern struct GameDriver asteroid_driver;
extern struct GameDriver asteroi1_driver;
extern struct GameDriver astdelux_driver;
extern struct GameDriver astdelu1_driver;
extern struct GameDriver bwidow_driver;
extern struct GameDriver bzone_driver;
extern struct GameDriver bzone2_driver;
extern struct GameDriver gravitar_driver;
extern struct GameDriver gravitr2_driver;
extern struct GameDriver llander_driver;
extern struct GameDriver llander1_driver;
extern struct GameDriver redbaron_driver;
extern struct GameDriver spacduel_driver;
extern struct GameDriver tempest_driver;
extern struct GameDriver tempest1_driver;
extern struct GameDriver tempest2_driver;
extern struct GameDriver temptube_driver;
extern struct GameDriver starwars_driver;
extern struct GameDriver starwar1_driver;
extern struct GameDriver empire_driver;
extern struct GameDriver mhavoc_driver;
extern struct GameDriver mhavoc2_driver;
extern struct GameDriver mhavocrv_driver;
extern struct GameDriver quantum_driver;
extern struct GameDriver quantum1_driver;
extern struct GameDriver quantump_driver;

/* Atari "Centipede hardware" games */
extern struct GameDriver warlord_driver;
extern struct GameDriver centiped_driver;
extern struct GameDriver centipd2_driver;
extern struct GameDriver centipdb_driver;
extern struct GameDriver milliped_driver;
extern struct GameDriver qwakprot_driver;

/* Atari "Kangaroo hardware" games */
extern struct GameDriver kangaroo_driver;
extern struct GameDriver kangarob_driver;
extern struct GameDriver arabian_driver;
extern struct GameDriver arabiana_driver;

/* Atari "Missile Command hardware" games */
extern struct GameDriver missile_driver;
extern struct GameDriver missile2_driver;
extern struct GameDriver suprmatk_driver;

/* Atari b/w games */
extern struct GameDriver sprint1_driver;
extern struct GameDriver sprint2_driver;
extern struct GameDriver sbrkout_driver;
extern struct GameDriver dominos_driver;
extern struct GameDriver nitedrvr_driver;
extern struct GameDriver bsktball_driver;
extern struct GameDriver copsnrob_driver;
extern struct GameDriver avalnche_driver;
extern struct GameDriver subs_driver;
extern struct GameDriver atarifb_driver;
extern struct GameDriver atarifb1_driver;
extern struct GameDriver atarifb4_driver;
extern struct GameDriver abaseb_driver;
extern struct GameDriver abaseb2_driver;
extern struct GameDriver canyon_driver;
extern struct GameDriver canbprot_driver;
extern struct GameDriver skydiver_driver;

/* misc Atari games */
extern struct GameDriver polepos_driver;
extern struct GameDriver foodf_driver;
extern struct GameDriver liberatr_driver;
extern struct GameDriver ccastles_driver;
extern struct GameDriver ccastle2_driver;
extern struct GameDriver cloak_driver;
extern struct GameDriver cloud9_driver;
extern struct GameDriver jedi_driver;

/* Atari System 1 games */
extern struct GameDriver marble_driver;
extern struct GameDriver marble2_driver;
extern struct GameDriver marblea_driver;
extern struct GameDriver peterpak_driver;
extern struct GameDriver indytemp_driver;
extern struct GameDriver indytem2_driver;
extern struct GameDriver roadrunn_driver;
extern struct GameDriver roadblst_driver;

/* Atari System 2 games */
extern struct GameDriver paperboy_driver;
extern struct GameDriver ssprint_driver;
extern struct GameDriver csprint_driver;
extern struct GameDriver a720_driver;
extern struct GameDriver a720b_driver;
extern struct GameDriver apb_driver;
extern struct GameDriver apb2_driver;

/* later Atari games */
extern struct GameDriver gauntlet_driver;
extern struct GameDriver gauntir1_driver;
extern struct GameDriver gauntir2_driver;
extern struct GameDriver gaunt2p_driver;
extern struct GameDriver gaunt2_driver;
extern struct GameDriver vindctr2_driver;
extern struct GameDriver atetris_driver;
extern struct GameDriver atetrisa_driver;
extern struct GameDriver atetrisb_driver;
extern struct GameDriver atetcktl_driver;
extern struct GameDriver atetckt2_driver;
extern struct GameDriver toobin_driver;
extern struct GameDriver vindictr_driver;
extern struct GameDriver klax_driver;
extern struct GameDriver klax2_driver;
extern struct GameDriver klax3_driver;
extern struct GameDriver blstroid_driver;
extern struct GameDriver blstroi2_driver;
extern struct GameDriver xybots_driver;
extern struct GameDriver eprom_driver;
extern struct GameDriver eprom2_driver;
extern struct GameDriver skullxbo_driver;
extern struct GameDriver skullxb2_driver;
extern struct GameDriver badlands_driver;
extern struct GameDriver cyberb2p_driver;
extern struct GameDriver rampart_driver;
extern struct GameDriver ramprt2p_driver;
extern struct GameDriver shuuz_driver;
extern struct GameDriver shuuz2_driver;

/* SNK / Rock-ola games */
extern struct GameDriver sasuke_driver;
extern struct GameDriver satansat_driver;
extern struct GameDriver zarzon_driver;
extern struct GameDriver vanguard_driver;
extern struct GameDriver vangrdce_driver;
extern struct GameDriver fantasy_driver;
extern struct GameDriver pballoon_driver;
extern struct GameDriver nibbler_driver;
extern struct GameDriver nibblera_driver;

/* later SNK games */
extern struct GameDriver tnk3_driver;
extern struct GameDriver tnk3j_driver;
extern struct GameDriver aso_driver;
extern struct GameDriver athena_driver;
extern struct GameDriver fitegolf_driver;
extern struct GameDriver ikari_driver;
extern struct GameDriver ikarijp_driver;
extern struct GameDriver ikarijpb_driver;
extern struct GameDriver victroad_driver;
extern struct GameDriver dogosoke_driver;
extern struct GameDriver gwar_driver;
extern struct GameDriver bermudat_driver;
extern struct GameDriver psychos_driver;
extern struct GameDriver psychosa_driver;
extern struct GameDriver chopper_driver;
extern struct GameDriver legofair_driver;
extern struct GameDriver tdfever_driver;
extern struct GameDriver tdfeverj_driver;
extern struct GameDriver pow_driver;
extern struct GameDriver powj_driver;
extern struct GameDriver prehisle_driver;
extern struct GameDriver prehislu_driver;
extern struct GameDriver prehislj_driver;

/* Technos games */
extern struct GameDriver mystston_driver;
extern struct GameDriver matmania_driver;
extern struct GameDriver excthour_driver;
extern struct GameDriver maniach_driver;
extern struct GameDriver maniach2_driver;
extern struct GameDriver renegade_driver;
extern struct GameDriver kuniokub_driver;
extern struct GameDriver xsleena_driver;
extern struct GameDriver xsleenab_driver;
extern struct GameDriver solarwar_driver;
extern struct GameDriver battlane_driver;
extern struct GameDriver battlan2_driver;
extern struct GameDriver battlan3_driver;
extern struct GameDriver ddragon_driver;
extern struct GameDriver ddragonb_driver;
extern struct GameDriver ddragon2_driver;
extern struct GameDriver blockout_driver;
extern struct GameDriver blckout2_driver;

/* Stern "Berzerk hardware" games */
extern struct GameDriver berzerk_driver;
extern struct GameDriver berzerk1_driver;
extern struct GameDriver frenzy_driver;

/* GamePlan games */
extern struct GameDriver megatack_driver;
extern struct GameDriver killcom_driver;
extern struct GameDriver challeng_driver;
extern struct GameDriver kaos_driver;

/* "stratovox hardware" games */
extern struct GameDriver route16_driver;
extern struct GameDriver stratvox_driver;
extern struct GameDriver stratvxb_driver;
extern struct GameDriver speakres_driver;

/* Zaccaria games */
extern struct GameDriver monymony_driver;
extern struct GameDriver jackrabt_driver;
extern struct GameDriver jackrab2_driver;
extern struct GameDriver jackrabs_driver;

/* UPL games */
extern struct GameDriver nova2001_driver;
extern struct GameDriver pkunwar_driver;
extern struct GameDriver pkunwarj_driver;
extern struct GameDriver ninjakd2_driver;
extern struct GameDriver ninjak2a_driver;
extern struct GameDriver ninjak2b_driver;
extern struct GameDriver rdaction_driver;
extern struct GameDriver mnight_driver;

/* Williams/Midway TMS34010 games */
extern struct GameDriver narc_driver;
extern struct GameDriver trog_driver;
extern struct GameDriver trog3_driver;
extern struct GameDriver trogp_driver;
extern struct GameDriver smashtv_driver;
extern struct GameDriver smashtv6_driver;
extern struct GameDriver smashtv5_driver;
extern struct GameDriver smashtv4_driver;
extern struct GameDriver hiimpact_driver;
extern struct GameDriver shimpact_driver;
extern struct GameDriver strkforc_driver;
extern struct GameDriver mk_driver;
extern struct GameDriver mkla1_driver;
extern struct GameDriver mkla2_driver;
extern struct GameDriver term2_driver;
extern struct GameDriver totcarn_driver;
extern struct GameDriver totcarnp_driver;
extern struct GameDriver mk2_driver;
extern struct GameDriver mk2r32_driver;
extern struct GameDriver mk2r14_driver;
extern struct GameDriver nbajam_driver;

/* Cinematronics raster games */
extern struct GameDriver jack_driver;
extern struct GameDriver jack2_driver;
extern struct GameDriver jack3_driver;
extern struct GameDriver treahunt_driver;
extern struct GameDriver zzyzzyxx_driver;
extern struct GameDriver zzyzzyx2_driver;
extern struct GameDriver brix_driver;
extern struct GameDriver sucasino_driver;

/* Cinematronics vector games */
extern struct GameDriver spacewar_driver;
extern struct GameDriver barrier_driver;
extern struct GameDriver starcas_driver;
extern struct GameDriver tgunner_driver;
extern struct GameDriver ripoff_driver;
extern struct GameDriver armora_driver;
extern struct GameDriver wotw_driver;
extern struct GameDriver warrior_driver;
extern struct GameDriver starhawk_driver;
extern struct GameDriver solarq_driver;
extern struct GameDriver boxingb_driver;
extern struct GameDriver speedfrk_driver;
extern struct GameDriver sundance_driver;
extern struct GameDriver demon_driver;

/* "The Pit hardware" games */
extern struct GameDriver roundup_driver;
extern struct GameDriver fitter_driver;
extern struct GameDriver thepit_driver;
extern struct GameDriver intrepid_driver;
extern struct GameDriver intrepi2_driver;
extern struct GameDriver portman_driver;
extern struct GameDriver suprmous_driver;
extern struct GameDriver suprmou2_driver;

/* Valadon Automation games */
extern struct GameDriver bagman_driver;
extern struct GameDriver bagnard_driver;
extern struct GameDriver bagmans_driver;
extern struct GameDriver bagmans2_driver;
extern struct GameDriver sbagman_driver;
extern struct GameDriver sbagmans_driver;
extern struct GameDriver pickin_driver;

/* Seibu Denshi / Seibu Kaihatsu games */
extern struct GameDriver stinger_driver;
extern struct GameDriver scion_driver;
extern struct GameDriver scionc_driver;
extern struct GameDriver wiz_driver;

/* Jaleco games */
extern struct GameDriver exerion_driver;
extern struct GameDriver exeriont_driver;
extern struct GameDriver exerionb_driver;
extern struct GameDriver formatz_driver;
extern struct GameDriver aeroboto_driver;
extern struct GameDriver citycon_driver;
extern struct GameDriver citycona_driver;
extern struct GameDriver cruisin_driver;
extern struct GameDriver psychic5_driver;

/* Jaleco Mega System 1 games */
extern struct GameDriver lomakaj_driver;
extern struct GameDriver p47_driver;
extern struct GameDriver p47j_driver;
extern struct GameDriver street64_driver;
extern struct GameDriver edf_driver;
extern struct GameDriver rodlandj_driver;
extern struct GameDriver avspirit_driver;
extern struct GameDriver astyanax_driver;
extern struct GameDriver hachoo_driver;
extern struct GameDriver plusalph_driver;
extern struct GameDriver phantasm_driver;
extern struct GameDriver stdragon_driver;

/* Video System Co. games */
extern struct GameDriver pspikes_driver;
extern struct GameDriver turbofrc_driver;
extern struct GameDriver aerofgt_driver;
extern struct GameDriver aerofgtb_driver;
extern struct GameDriver aerofgtc_driver;

/* Leland games */
extern struct GameDriver wseries_driver;
extern struct GameDriver basebal2_driver;
extern struct GameDriver dblplay_driver;
extern struct GameDriver offroad_driver;
extern struct GameDriver offroadt_driver;
extern struct GameDriver teamqb_driver;
extern struct GameDriver strkzone_driver;
extern struct GameDriver pigout_driver;
extern struct GameDriver pigoutj_driver;
extern struct GameDriver redlin2p_driver;
extern struct GameDriver viper_driver;
extern struct GameDriver aafb_driver;
extern struct GameDriver aafb2p_driver;
extern struct GameDriver ataxx_driver;

/* Tad games */
extern struct GameDriver cabal2_driver;
extern struct GameDriver cabalbl_driver;
extern struct GameDriver toki_driver;
extern struct GameDriver toki2_driver;
extern struct GameDriver toki3_driver;
extern struct GameDriver tokiu_driver;
extern struct GameDriver tokib_driver;

/* Orca games */
extern struct GameDriver marineb_driver;
extern struct GameDriver changes_driver;
extern struct GameDriver springer_driver;
extern struct GameDriver hoccer_driver;
extern struct GameDriver hoccer2_driver;
extern struct GameDriver funkybee_driver;
extern struct GameDriver skylancr_driver;
extern struct GameDriver zodiack_driver;
extern struct GameDriver dogfight_driver;
extern struct GameDriver moguchan_driver;
extern struct GameDriver espial_driver;
extern struct GameDriver espiale_driver;

extern struct GameDriver spacefb_driver;
extern struct GameDriver spacefbg_driver;
extern struct GameDriver spacebrd_driver;
extern struct GameDriver spacedem_driver;
extern struct GameDriver blueprnt_driver;
extern struct GameDriver omegrace_driver;
extern struct GameDriver vastar_driver;
extern struct GameDriver vastar2_driver;
extern struct GameDriver tankbatt_driver;
extern struct GameDriver dday_driver;
extern struct GameDriver ddayc_driver;
extern struct GameDriver cabal_driver;
extern struct GameDriver gundealr_driver;
extern struct GameDriver gundeala_driver;
extern struct GameDriver yamyam_driver;
extern struct GameDriver wiseguy_driver;
extern struct GameDriver leprechn_driver;
extern struct GameDriver potogold_driver;
extern struct GameDriver hexa_driver;
extern struct GameDriver redalert_driver;
extern struct GameDriver irobot_driver;
extern struct GameDriver spiders_driver;
extern struct GameDriver spiders2_driver;
extern struct GameDriver wanted_driver;
extern struct GameDriver stactics_driver;
extern struct GameDriver goldstar_driver;
extern struct GameDriver goldstbl_driver;
extern struct GameDriver exterm_driver;
extern struct GameDriver sharkatt_driver;
extern struct GameDriver kingofb_driver;
extern struct GameDriver kingofbj_driver;
extern struct GameDriver ringking_driver;
extern struct GameDriver ringkin2_driver;
extern struct GameDriver ringkin3_driver;
extern struct GameDriver zerozone_driver;
extern struct GameDriver exctsccr_driver;
extern struct GameDriver exctscca_driver;
extern struct GameDriver exctsccb_driver;
extern struct GameDriver speedbal_driver;
extern struct GameDriver sauro_driver;
extern struct GameDriver galpanic_driver;
extern struct GameDriver airbustr_driver;
extern struct GameDriver ambush_driver;
extern struct GameDriver starcrus_driver;
extern struct GameDriver sichuan2_driver;
extern struct GameDriver shisen_driver;
extern struct GameDriver goindol_driver;
extern struct GameDriver homo_driver;
extern struct GameDriver dlair_driver;

#endif /* NEOMAME */

#ifndef NEOFREE

/* Neo Geo games */
extern struct GameDriver nam1975_driver;
extern struct GameDriver joyjoy_driver;
extern struct GameDriver mahretsu_driver;
extern struct GameDriver cyberlip_driver;
extern struct GameDriver tpgolf_driver;
extern struct GameDriver ridhero_driver;
extern struct GameDriver bstars_driver;
extern struct GameDriver bstars2_driver;
extern struct GameDriver ttbb_driver;
extern struct GameDriver lbowling_driver;
extern struct GameDriver superspy_driver;
extern struct GameDriver legendos_driver;
extern struct GameDriver socbrawl_driver;
extern struct GameDriver roboarmy_driver;
extern struct GameDriver alpham2_driver;
extern struct GameDriver eightman_driver;
extern struct GameDriver burningf_driver;
extern struct GameDriver kotm_driver;
extern struct GameDriver kotm2_driver;
extern struct GameDriver gpilots_driver;
extern struct GameDriver sengoku_driver;
extern struct GameDriver sengoku2_driver;
extern struct GameDriver lresort_driver;
extern struct GameDriver fbfrenzy_driver;
extern struct GameDriver mutnat_driver;
extern struct GameDriver countb_driver;
extern struct GameDriver tophuntr_driver;
extern struct GameDriver aof_driver;
extern struct GameDriver aof2_driver;
extern struct GameDriver aof3_driver;
extern struct GameDriver fatfury1_driver;
extern struct GameDriver fatfury2_driver;
extern struct GameDriver fatfursp_driver;
extern struct GameDriver fatfury3_driver;
extern struct GameDriver rbff1_driver;
extern struct GameDriver rbffspec_driver;
extern struct GameDriver rbff2_driver;
extern struct GameDriver kof94_driver;
extern struct GameDriver kof95_driver;
extern struct GameDriver kof96_driver;
extern struct GameDriver kof97_driver;
extern struct GameDriver kof98_driver;
extern struct GameDriver savagere_driver;
extern struct GameDriver kizuna_driver;
extern struct GameDriver samsho_driver;
extern struct GameDriver samsho2_driver;
extern struct GameDriver samsho3_driver;
extern struct GameDriver samsho4_driver;
extern struct GameDriver lastblad_driver;
extern struct GameDriver lastbld2_driver;
extern struct GameDriver ssideki_driver;
extern struct GameDriver ssideki2_driver;
extern struct GameDriver ssideki3_driver;
extern struct GameDriver ssideki4_driver;
extern struct GameDriver mslug2_driver;
extern struct GameDriver bjourney_driver;
extern struct GameDriver maglord_driver;
extern struct GameDriver maglordh_driver;
extern struct GameDriver ncombat_driver;
extern struct GameDriver crsword_driver;
extern struct GameDriver trally_driver;
extern struct GameDriver ncommand_driver;
extern struct GameDriver wh1_driver;
extern struct GameDriver wh2_driver;
extern struct GameDriver wh2j_driver;
extern struct GameDriver whp_driver;
extern struct GameDriver aodk_driver;
extern struct GameDriver ninjamas_driver;
extern struct GameDriver overtop_driver;
extern struct GameDriver twinspri_driver;
extern struct GameDriver janshin_driver;
extern struct GameDriver pulstar_driver;
extern struct GameDriver spinmast_driver;
extern struct GameDriver karnovr_driver;
extern struct GameDriver wjammers_driver;
extern struct GameDriver strhoops_driver;
extern struct GameDriver magdrop2_driver;
extern struct GameDriver magdrop3_driver;
extern struct GameDriver gururin_driver;
extern struct GameDriver miexchng_driver;
extern struct GameDriver panicbom_driver;
extern struct GameDriver kabukikl_driver;
extern struct GameDriver neobombe_driver;
extern struct GameDriver minasan_driver;
extern struct GameDriver bakatono_driver;
extern struct GameDriver mslug_driver;
extern struct GameDriver turfmast_driver;
extern struct GameDriver zedblade_driver;
extern struct GameDriver viewpoin_driver;
extern struct GameDriver quizkof_driver;
extern struct GameDriver stakwin_driver;
extern struct GameDriver stakwin2_driver;
extern struct GameDriver ragnagrd_driver;
extern struct GameDriver shocktro_driver;
extern struct GameDriver galaxyfg_driver;
extern struct GameDriver wakuwak7_driver;
extern struct GameDriver pbobble_driver;
extern struct GameDriver marukodq_driver;
extern struct GameDriver doubledr_driver;
extern struct GameDriver gowcaizr_driver;
extern struct GameDriver tws96_driver;
extern struct GameDriver blazstar_driver;
extern struct GameDriver pspikes2_driver;
extern struct GameDriver sonicwi2_driver;
extern struct GameDriver sonicwi3_driver;
extern struct GameDriver androdun_driver;
extern struct GameDriver goalx3_driver;
extern struct GameDriver puzzledp_driver;
extern struct GameDriver puzzldpr_driver;
extern struct GameDriver neodrift_driver;
extern struct GameDriver neomrdo_driver;
extern struct GameDriver breakers_driver;
extern struct GameDriver breakrev_driver;
extern struct GameDriver flipshot_driver;

#endif /* NEOFREE */


const struct GameDriver *drivers[] =
{
#ifndef NEOMAME
	/* "Pacman hardware" games */
	&pacman_driver,		/* (c) 1980 Namco */
	&pacmanjp_driver,	/* (c) 1980 Namco */
	&pacmanm_driver,	/* (c) 1980 Midway */
	&npacmod_driver,	/* (c) 1981 Namco */
	&pacmod_driver,		/* (c) 1981 Midway */
	&hangly_driver,		/* hack */
	&hangly2_driver,	/* hack */
	&puckman_driver,	/* hack */
	&pacheart_driver,	/* hack */
	&piranha_driver,	/* hack */
	&pacplus_driver,
	&mspacman_driver,	/* (c) 1981 Midway (but it's a bootleg) */	/* made by Gencomp */
	&mspacatk_driver,	/* hack */
	&pacgal_driver,		/* hack */
	&maketrax_driver,	/* (c) 1981 Williams, high score table says KRL (fur Kural) */
	&crush_driver,		/* (c) 1981 Kural Samno Electric Ltd */
	&crush2_driver,		/* (c) 1981 Kural Esco Electric Ltd - bootleg? */
	&crush3_driver,		/* Kural Electric Ltd - bootleg? */
	&mbrush_driver,		/* 1981 bootleg */
	&eyes_driver,		/* (c) 1982 Digitrex Techstar + "Rockola presents" */
	&mrtnt_driver,		/* (c) 1983 Telko */
	&ponpoko_driver,	/* (c) 1982 Sigma Ent. Inc. */
	&lizwiz_driver,		/* (c) 1985 Techstar + "Sunn presents" */
	&theglob_driver,	/* (c) 1983 Epos Corporation */
	&beastf_driver,		/* (c) 1984 Epos Corporation */
	&jumpshot_driver,
	&vanvan_driver,		/* (c) 1983 Karateco */
	&vanvanb_driver,	/* bootleg */
	&alibaba_driver,	/* (c) 1982 Sega */
	&pengo_driver,		/* 834-0386 (c) 1982 Sega */
	&pengo2_driver,		/* 834-0386 (c) 1982 Sega */
	&pengo2u_driver,	/* 834-0386 (c) 1982 Sega */
	&penta_driver,		/* bootleg */
	&jrpacman_driver,	/* (c) 1983 Midway */

	/* "Galaxian hardware" games */
	&galaxian_driver,	/* (c) Namco */
	&galmidw_driver,	/* (c) Midway */
	&superg_driver,		/* hack */
	&galaxb_driver,		/* bootleg */
	&galapx_driver,		/* hack */
	&galap1_driver,		/* hack */
	&galap4_driver,		/* hack */
	&galturbo_driver,	/* hack */
	&pisces_driver,		/* ? */
	&japirem_driver,	/* (c) Irem */
	&uniwars_driver,	/* (c) Karateco (bootleg?) */
	&spacbatt_driver,	/* bootleg */
	&warofbug_driver,	/* (c) 1981 Armenia */
	&redufo_driver,		/* ? */
	&pacmanbl_driver,	/* bootleg */
	&devilfsh_driver,	/* (c) 1984 Vision / Artic */
	&zigzag_driver,		/* (c) 1982 LAX */
	&zigzag2_driver,	/* (c) 1982 LAX */
	&jumpbug_driver,	/* (c) 1981 Rock-ola */
	&jumpbugb_driver,	/* (c) 1981 Sega */
	&mooncrgx_driver,	/* bootleg */
	&mooncrst_driver,	/* (c) 1980 Nichibutsu */
	&mooncrsg_driver,	/* (c) 1980 Gremlin */
	&mooncrsb_driver,	/* bootleg */
	&mooncrs2_driver,	/* bootleg */
	&fantazia_driver,	/* bootleg */
	&eagle_driver,		/* (c) Centuri */
	&eagle2_driver,		/* (c) Centuri */
	&moonqsr_driver,	/* (c) 1980 Nichibutsu */
	&checkman_driver,	/* (c) 1982 Zilec-Zenitone */
	&moonal2_driver,	/* Nichibutsu */
	&moonal2b_driver,	/* Nichibutsu */
	&kingball_driver,	/* (c) 1980 Namco */
	&kingbalj_driver,	/* (c) 1980 Namco */

	/* "Scramble hardware" (and variations) games */
	&scramble_driver,	/* GX387 (c) 1981 Konami */
	&scrambls_driver,	/* GX387 (c) 1981 Stern */
	&scramblb_driver,	/* bootleg */
	&atlantis_driver,	/* (c) 1981 Comsoft */
	&theend_driver,		/* (c) 1980 Stern */
	&ckongs_driver,		/* bootleg */
	&froggers_driver,	/* bootleg */
	&amidars_driver,	/* (c) 1982 Konami */
	&triplep_driver,	/* (c) 1982 KKI */
	&mariner_driver,	/* (c) 1981 Amenip */
	&scobra_driver,		/* GX316 (c) 1981 Konami */
	&scobras_driver,	/* GX316 (c) 1981 Stern */
	&scobrab_driver,	/* GX316 (c) 1981 Karateco (bootleg?) */
	&stratgyx_driver,	/* GX306 (c) 1981 Stern */
	&stratgyb_driver,	/* GX306 bootleg (of the Konami version?) */
	&armorcar_driver,	/* (c) 1981 Stern */
	&armorca2_driver,	/* (c) 1981 Stern */
	&moonwar2_driver,	/* (c) 1981 Stern */
	&monwar2a_driver,	/* (c) 1981 Stern */
	&spdcoin_driver,	/* (c) 1984 Stern */
	&darkplnt_driver,	/* (c) 1982 Stern */
	&tazmania_driver,	/* (c) 1982 Stern */
	&tazmani2_driver,	/* (c) 1982 Stern */
	&calipso_driver,	/* (c) 1982 Tago */
	&anteater_driver,	/* (c) 1982 Tago */
	&rescue_driver,		/* (c) 1982 Stern */
	&minefld_driver,	/* (c) 1983 Stern */
	&losttomb_driver,	/* (c) 1982 Stern */
	&losttmbh_driver,	/* (c) 1982 Stern */
	&superbon_driver,	/* bootleg */
	&hustler_driver,	/* GX343 (c) 1981 Konami */
	&pool_driver,
	&billiard_driver,	/* bootleg */
	&frogger_driver,	/* 834-0068 (c) 1981 Sega */
	&frogsega_driver,	/* (c) 1981 Sega */
	&frogger2_driver,	/* 800-3110 (c) 1981 Sega */
	&amidar_driver,		/* GX337 (c) 1981 Konami */
	&amidaru_driver,	/* GX337 (c) 1982 Konami + Stern license */
	&amidaro_driver,	/* GX337 (c) 1982 Konami + Olympia license */
	&amigo_driver,		/* bootleg */
	&turtles_driver,	/* (c) 1981 Stern */
	&turpin_driver,		/* (c) 1981 Sega */
	&k600_driver,		/* GX353 (c) 1981 Konami */
	&flyboy_driver,		/* (c) 1982 Kaneko (bootleg?) */
	&fastfred_driver,	/* (c) 1982 Atari */
	&jumpcoas_driver,	/* (c) 1983 Kaneko */

	/* "Crazy Climber hardware" games */
	&cclimber_driver,	/* (c) 1980 Nichibutsu */
	&ccjap_driver,		/* (c) 1980 Nichibutsu */
	&ccboot_driver,		/* bootleg */
	&ccboot2_driver,	/* bootleg */
	&ckong_driver,		/* (c) 1981 Falcon */
	&ckonga_driver,		/* (c) 1981 Falcon */
	&ckongjeu_driver,	/* bootleg */
	&ckongo_driver,		/* bootleg */
	&ckongalc_driver,	/* bootleg */
	&monkeyd_driver,	/* bootleg */
	&rpatrolb_driver,	/* bootleg */
	&silvland_driver,	/* Falcon */
	&yamato_driver,		/* (c) 1983 Sega */
	&yamato2_driver,	/* (c) 1983 Sega */
	&swimmer_driver,	/* (c) 1982 Tehkan */
	&swimmera_driver,	/* (c) 1982 Tehkan */
	&guzzler_driver,	/* (c) 1983 Tehkan */

	/* Nichibutsu games */
	&friskyt_driver,	/* (c) 1981 */
	&radrad_driver,		/* (c) 1982 Nichibutsu USA */
	&seicross_driver,	/* (c) 1984 + Alice */
	&sectrzon_driver,	/* (c) 1984 + Alice */
	&wiping_driver,		/* (c) 1982 */
	&rugrats_driver,	/* (c) 1983 */
	&cop01_driver,		/* (c) 1985 */
	&cop01a_driver,		/* (c) 1985 */
	&terracre_driver,	/* (c) 1985 */
	&terracra_driver,	/* (c) 1985 */
	&galivan_driver,	/* (c) 1985 */
	&galivan2_driver,	/* (c) 1985 */
	&dangar_driver,		/* (c) 1986 */
	&dangar2_driver,	/* (c) 1986 */
	&dangarb_driver,	/* bootleg */
	&terraf_driver,		/* (c) 1987 */
	&terrafu_driver,	/* (c) 1987 Nichibutsu USA */
	&armedf_driver,		/* (c) 1988 */

	/* "Phoenix hardware" (and variations) games */
	&phoenix_driver,	/* (c) 1980 Amstar */
	&phoenixt_driver,	/* (c) 1980 Taito */
	&phoenix3_driver,	/* bootleg */
	&phoenixc_driver,	/* bootleg */
	&pleiads_driver,	/* (c) 1981 Tehkan */
	&pleiadce_driver,	/* (c) 1981 Centuri + Tehkan */
	&naughtyb_driver,	/* (c) 1982 Jaleco */
	&naughtya_driver,	/* bootleg */
	&naughtyc_driver,	/* (c) 1982 Jaleco + Cinematronics */
	&popflame_driver,	/* (c) 1982 Jaleco */
	&popflama_driver,	/* (c) 1982 Jaleco */

	/* Namco games */
	&warpwarp_driver,	/* (c) 1981 Namco */
	&warpwarr_driver,	/* (c) 1981 Rock-ola - different hardware */
						/* the high score table says "NAMCO" */
	&warpwar2_driver,	/* (c) 1981 Rock-ola - different hardware */
						/* the high score table says "NAMCO" */
	&rallyx_driver,		/* (c) 1980 Namco */
	&rallyxm_driver,	/* (c) 1980 Midway */
	&nrallyx_driver,	/* (c) 1981 Namco */
	&jungler_driver,	/* GX327 (c) 1981 Konami */
	&junglers_driver,	/* GX327 (c) 1981 Stern */
	&locomotn_driver,	/* GX359 (c) 1982 Konami + Centuri license */
	&commsega_driver,	/* (c) 1983 Sega */
	/* the following ones all have a custom I/O chip */
	&bosco_driver,		/* (c) 1981 */
	&boscomd_driver,	/* (c) 1981 Midway */
	&boscomd2_driver,	/* (c) 1981 Midway */
	&galaga_driver,		/* (c) 1981 */
	&galagamw_driver,	/* (c) 1981 Midway */
	&galagads_driver,	/* hack */
	&gallag_driver,		/* bootleg */
	&galagab2_driver,	/* bootleg */
	&galaga84_driver,	/* hack */
	&digdug_driver,		/* (c) 1982 Atari */
	&digdugnm_driver,	/* (c) 1982 */
	&dzigzag_driver,	/* bootleg */
	&xevious_driver,	/* (c) 1982 */
	&xeviousa_driver,	/* (c) 1982 + Atari license */
	&xevios_driver,		/* bootleg */
	&sxevious_driver,	/* (c) 1984 */
	&superpac_driver,	/* (c) 1982 Midway */
	&superpcn_driver,	/* (c) 1982 */
	&pacnpal_driver,	/* (c) 1983 */
	&phozon_driver,		/* (c) 1983 */
	&mappy_driver,		/* (c) 1983 */
	&mappyjp_driver,	/* (c) 1983 */
	&digdug2_driver,	/* (c) 1985 */
	&todruaga_driver,	/* (c) 1984 */
	&todruagb_driver,	/* (c) 1984 */
	&motos_driver,		/* (c) 1985 */
	&grobda_driver,		/* (c) 1984 */
	&grobda2_driver,	/* (c) 1984 */
	&grobda3_driver,	/* (c) 1984 */
	&gaplus_driver,		/* (c) 1984 */
	&gaplusa_driver,	/* (c) 1984 */
	&galaga3_driver,	/* (c) 1984 */
	&galaga3a_driver,	/* (c) 1984 */
	/* no custom I/O in the following, HD63701 (or compatible) microcontroller instead */
	&pacland_driver,	/* (c) 1984 */
	&pacland2_driver,	/* (c) 1984 */
	&pacland3_driver,	/* (c) 1984 */
	&paclandm_driver,	/* (c) 1984 Midway */

	/* Namco System 86 games */
	/* 85.4  Hopping MAPPY (sequel to MAPPY) */
	/* 86.5  Sky Kid DX (sequel to Sky Kid) */
	&roishtar_driver,	/* (c) 1986 */
	&genpeitd_driver,	/* (c) 1986 */
	&rthunder_driver,	/* (c) 1986 */
	&rthundrb_driver,	/* (c) 1986 */
	&wndrmomo_driver,	/* (c) 1987 */

	/* Namco System 1 games */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&blazer_driver,
//	&ws90_driver,
//	&dspirits_driver,
//	&splatter_driver,
//	&galaga88_driver,
//	&pacmania_driver,
//	&alice_driver,
//	&shadowld_driver,

	/* Namco System 2 games */
//	&cosmogng_driver,
//	&assault_driver,
//	&phelios_driver,
//	&rthun2_driver,
//	&metlhawk_driver,

/*
>Namco games
>-----------------------------------------------------
>I will finish this mail by correcting Namco system board history
>in driver.c.
>
>-----------------------------------------------------
>Libble Rabble board
>83.12 Libble Rabble (The first Japanese game that uses M68K)
>86.4  Toy Pop
>
>-----------------------------------------------------
>PAC-LAND board
>84.8  PAC-LAND
>85.1  Dragon Buster
>
>-----------------------------------------------------
>Metro-Cross board
>85.5  Metro-Cross
>85.7  Baraduke
>
>-----------------------------------------------------
>System I Games
>87.4  [Youkai Douchuuki] (Shadow Land) - This game has no custom chip protection
>87.6  Dragon Spirit (OLD version)
>87.9  Blazer
>87.?? Dragon Spirit (NEW version)
>87.9  Quester
>87.11 PAC-MANIA
>87.12 Galaga'88
>88.3  Pro-[Yakyuu](baseball) World Stadium
>88.5  [CHOU-ZETU-RINN-JINN] [BERABOW]-MAN.
>88.7  Marchen Maze (Alice in Wonderland)
>88.8  [BAKU-TOTU-KIJYUU-TEI] (sequel to Baraduke)
>88.10 Pro Tennis World Court
>88.11 Splatter House
>88.12 Face Off
>89.2  Rompers
>89.3  Blast Off (sequel to Bosconian)
>89.1  Pro-[Yakyuu] World Stadium '89
>89.12 Dangerous Seed
>90.7  PRO-[YAKYUU] WORLD-STADIUM'90.
>90.10 Pistol[DAIMYOU NO BOUKEN] (sequel to Berabow-man)
>90.11 [SOUKO-BAN] DX
>91.12 Tank Force
>
>-----------------------------------------------------
>System II GAMES
>
>88.4  Assault
>88.9  Ordyne
>88.10 Metal Hawk (dual-system2)
>88.12 [MIRAI]-NINJA
>89.2  Phelios
>89.4  WALKURE [NO DEN-SETU](The legend of Valkyrie)
>89.6  Dirt Fox
>89.9  Finest Hour
>89.11 Burning Force
>90.2  Marvel Land
>90.5  [Kyuukai Douchuuki] (baseball game. uses character of Youkai
>Douchuuki)
>90.12 Dragon Saber
>91.3  Rolling Thunder 2
>91.3  Steel Gunner
>91.9  Super World Stadium
>92.3  Steel Gunner 2
>92.3  Cosmo Gangs the Video
>92.9  Super World Stadium '92 [GEKITOU-HEN]
>92.10 F/A


-----------------------------------------------------
NAMCO SYSTEM BOARD GAME HISTORY?(TABLE SIZE GAME FRAME)
-----------------------------------------------------

(NAMCO SYSTEM I)

87.4  [YOU-KAI-DOU-CHUU-KI]                      "YD"    NONE
87.6  DragonSpirit(OLD VERSION)                  "DS"    136
87.7  Blazer                                     "BZ"    144
87.?? DragonSpirit(NEW VERSION)                  "DS2"   136
87.9  Quester                                    "QS"    'on sub board(A)'
87.?? Quester(SPECIAL EDITION)                   "QS2"   'on sub board(A)'
87.11 PAC-MANIA                                  "PN"    151
??.?? PAC-MANIA(?)(it's rare in japan)           "PN2"   (151?)
87.12 Galaga'88                                  "G8"    153
88.3  '88 PRO-[YAKYUU] WORLD-STADIUM.            (WS?)   (154?)
88.5  [CHOU-ZETU-RINN-JINN] [BERABOW]-MAN.       "BM"    'on sub board(B)'
88.7  MELHEN MAZE(Alice in Wonderland)           "MM"    152
88.8  [BAKU-TOTU-KIJYUU-TEI]                     "BK"    155
88.10 pro-tennis WORLD COAT                      "WC"    143
88.11 splatter house                             "SH"    181
88.12 FAIS OFF(spell-mistake?)                   "FO"    'on sub board(C)'
89.2  ROMPERS                                    "RP"    182
89.3  BLAST-OFF                                  "BO"    183
89.1  PRO-[YAKYUU] WORLD-STADIUM'89.             "W9"    184
89.12 DANGERUS SEED                              "DR"    308
90.7  PRO-[YAKYUU] WORLD-STADIUM'90.             "W90"   310
90.10 [PISUTORU-DAIMYOU NO BOUKEN]               "PD"    309
90.11 [SOUKO-BAN] DX                             "SB"    311
91.12 TANK-FOURCE                                "TF"    185

-----------------------------------------------------

About system I sub board:
sub board used game always need sub board.
if sub bord nothing then not starting game.
because, key custum chip on sub board.
(diffalent type key custum chip on mother board.)

-----------------------------------------------------

'sub board(A)'= for sencer interface bord.
it having changed jp that used format taito/namco.

-----------------------------------------------------

'sub board(B)'= two toggle switch bord.
it sub board the inter face that 'two toggle switch'.
( == in japan called 'BERABOW-SWITCH'.)

<push switch side view>

  =~~~~~=       =~~~~~=
   |   |         |   |
   +---+         +---+
    | |           |||

NORMAL-SW     BERABOW-SW
(two-pins)    (tree-pins)
(GND?,sw1)     (GND?,sw1,sw2)

It abnormal switch was can feel player pushed power.
(It power in proportion to the push speed.)
It used the game controls, 1 stick & 2 it botton game.
Passaged over-time from sw1 triggerd to sw2 triggerd then
min panch/kick,and short time then max panch/kick feeled the game.

-----------------------------------------------------

'sub board(C)'= can 4 players sub board.
it subboard on 3 player & 4 player input lines.

-----------------------------------------------------
( into '[' to ']' mark is 'NI-HO-N-GO'.(japannese languege) )
-----------------------------------------------------
System 2 GAMES

88.4  ASSULT
88.9  ORDYNE(spell-mistake?)
88.10 METAL HAWK(dual-system2)
88.12 [MIRAI]-NINJA
89.2  Phelious(spell-mistake?)
89.4  WALKURE [NO DEN-SETU](the legend of WALKURE)
89.6  (??durt-fox)
89.9  FINEST HOUR
89.11 BURNING FORCE
90.2  MARBEL-LAND
90.5  (??KYUU-KAI-DOU-CHUU-KI)
90.12 DRAGON-SAVER
91.3  ROLLING-THUNDER2
91.3  (??STEEL-GUNNER)
91.9  (??SUPER-WORLD-STUDIUM)
92.3  (??STEEL-GUNNER2)
92.3  COSMO-GANGS THE VIDEO
92.9  (??SUPER-WORLD-STUDIUM'92 [GEKITOU-HEN])

*/

	/* Universal games */
	&cosmicg_driver,	/* (c) 1979 */
	&cosmica_driver,	/* (c) [1979] */
	&panic_driver,		/* (c) 1980 */
	&panica_driver,		/* (c) 1980 */
	&panicger_driver,	/* (c) 1980 */
	&magspot2_driver,	/* (c) [1980?] */
	&devzone_driver,	/* (c) [1980?] */
	&nomnlnd_driver,	/* (c) [1980?] */
	&nomnlndg_driver,	/* (c) [1980?] Universal + Gottlieb license */
	&cheekyms_driver,	/* (c) [1980?] */
	&ladybug_driver,	/* (c) 1981 */
	&ladybugb_driver,	/* bootleg */
	&snapjack_driver,	/* (c) */
	&cavenger_driver,	/* (c) 1981 */
	&mrdo_driver,		/* (c) 1982 */
	&mrdot_driver,		/* (c) 1982 + Taito license */
	&mrdofix_driver,	/* (c) 1982 + Taito license */
	&mrlo_driver,		/* bootleg */
	&mrdu_driver,		/* bootleg */
	&mrdoy_driver,		/* bootleg */
	&yankeedo_driver,	/* bootleg */
	&docastle_driver,	/* (c) 1983 */
	&docastl2_driver,	/* (c) 1983 */
	&dounicorn_driver,	/* (c) 1983 */
	&dorunrun_driver,	/* (c) 1984 */
	&dorunru2_driver,	/* (c) 1984 */
	&dorunruc_driver,	/* (c) 1984 */
	&spiero_driver,		/* (c) 1987 */
	&dowild_driver,		/* (c) 1984 */
	&jjack_driver,		/* (c) 1984 */
	&kickridr_driver,	/* (c) 1984 */

	/* Nintendo games */
	&radarscp_driver,	/* (c) 1980 Nintendo */
	&dkong_driver,		/* (c) 1981 Nintendo of America */
	&dkongjp_driver,	/* (c) 1981 Nintendo */
	&dkongjr_driver,	/* (c) 1982 Nintendo of America */
	&dkngjrjp_driver,	/* no copyright notice */
	&dkjrjp_driver,		/* (c) 1982 Nintendo */
	&dkjrbl_driver,		/* (c) 1982 Nintendo of America */
	&dkong3_driver,		/* (c) 1983 Nintendo of America */
	&mario_driver,		/* (c) 1983 Nintendo of America */
	&masao_driver,		/* bootleg */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&hunchy_driver,		/* hacked Donkey Kong board */
//	&herocast_driver,
	&popeye_driver,
	&popeye2_driver,
	&popeyebl_driver,	/* bootleg */
	&punchout_driver,	/* (c) 1984 */
	&spnchout_driver,	/* (c) 1984 */
	&armwrest_driver,	/* (c) 1985 */

	/* Midway 8080 b/w games */
	&seawolf_driver,	/* 596 [1976] */
	&gunfight_driver,	/* 597 [1975] */
	/* 603 - Top Gun [1976] */
	&tornbase_driver,	/* 605 [1976] */
	&zzzap_driver,		/* 610 [1976] */
	&maze_driver,		/* 611 [1976] */
	&boothill_driver,	/* 612 [1977] */
	&checkmat_driver,	/* 615 [1977] */
	/* 618 - Road Runner [1977] */
	/* 618 - Desert Gun [1977] */
	&dplay_driver,		/* 619 [1977] */
	&lagunar_driver,	/* 622 [1977] */
	&gmissile_driver,	/* 623 [1977] */
	&m4_driver,			/* 626 [1977] */
	&clowns_driver,		/* 630 [1978] */
	/* 640 - Space Walk [1978] */
	&einnings_driver,	/* 642 [1978] Midway */
	/* 643 - Shuffleboard [1978] */
	&dogpatch_driver,	/* 644 [1977] */
	&spcenctr_driver,	/* 645 (c) 1980 Midway */
	&phantom2_driver,	/* 652 [1979] */
	&midwbowl_driver,	/* 730 [1978] Midway */
	&invaders_driver,	/* 739 [1979] */
	&blueshrk_driver,	/* 742 [1978] */
	/* 851 - Space Invaders II cocktail */
	&invdelux_driver,	/* 852 [1980] Midway */
	&invadpt2_driver,	/* 852 [1980] Taito */
	/* 870 - Space Invaders Deluxe cocktail */
	&earthinv_driver,
	&spaceatt_driver,
	&sinvemag_driver,
	&invrvnge_driver,
	&invrvnga_driver,
	&galxwars_driver,
	&lrescue_driver,	/* (c) 1979 Taito */
	&grescue_driver,	/* bootleg? */
	&desterth_driver,	/* bootleg */
	&cosmicmo_driver,	/* Universal */
	&rollingc_driver,	/* Nichibutsu */
	&bandido_driver,	/* (c) Exidy */
	&ozmawars_driver,	/* Shin Nihon Kikaku (SNK) */
	&solfight_driver,	/* bootleg */
	&spaceph_driver,	/* Zilec Games */
	&schaser_driver,	/* Taito */
	&lupin3_driver,		/* (c) 1980 Taito */
	&helifire_driver,	/* (c) Nintendo */
	&helifira_driver,	/* (c) Nintendo */
	&spacefev_driver,
	&sfeverbw_driver,
	&astlaser_driver,
	&intruder_driver,
	&polaris_driver,	/* (c) 1980 Taito */
	&polarisa_driver,	/* (c) 1980 Taito */
	&ballbomb_driver,	/* (c) 1980 Taito */
	&m79amb_driver,
	&alieninv2_driver,
	&sitv_driver,
	&sicv_driver,
	&sisv_driver,

	/* "Midway" Z80 b/w games */
	&astinvad_driver,	/* (c) 1980 Stern */
	&kamikaze_driver,	/* Leijac Corporation */
	&spaceint_driver,	/* [1980] Shoei */

	/* Meadows S2650 games */
	&lazercmd_driver,	/* [1976?] */
	&deadeye_driver,	/* [1978?] */
	&gypsyjug_driver,	/* [1978?] */
	&medlanes_driver,	/* [1977?] */

	/* Midway "Astrocade" games */
	&wow_driver,		/* (c) 1980 */
	&robby_driver,		/* (c) 1981 */
	&gorf_driver,		/* (c) 1981 */
	&gorfpgm1_driver,	/* (c) 1981 */
	&seawolf2_driver,
	&spacezap_driver,	/* (c) 1980 */
	&ebases_driver,

	/* Bally Midway MCR games */
	/* MCR1 */
	&solarfox_driver,	/* (c) 1981 */
	&kick_driver,		/* (c) 1981 */
	&kicka_driver,		/* bootleg? */
	/* MCR2 */
	&shollow_driver,	/* (c) 1981 */
	&shollow2_driver,	/* (c) 1981 */
	&tron_driver,		/* (c) 1982 */
	&tron2_driver,		/* (c) 1982 */
	&kroozr_driver,		/* (c) 1982 */
	&domino_driver,		/* (c) 1982 */
	&wacko_driver,		/* (c) 1982 */
	&twotiger_driver,	/* (c) 1984 */
	/* MCR2 + MCR3 sprites */
	&journey_driver,	/* (c) 1983 */
	/* MCR3 */
	&tapper_driver,		/* (c) 1983 */
	&sutapper_driver,	/* (c) 1983 */
	&rbtapper_driver,	/* (c) 1984 */
	&dotron_driver,		/* (c) 1983 */
	&dotrone_driver,	/* (c) 1983 */
	&destderb_driver,	/* (c) 1984 */
	&timber_driver,		/* (c) 1984 */
	&spyhunt_driver,	/* (c) 1983 */
	&crater_driver,		/* (c) 1984 */
	&sarge_driver,		/* (c) 1985 */
	&rampage_driver,	/* (c) 1986 */
	&rampage2_driver,	/* (c) 1986 */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&powerdrv_driver,
	&maxrpm_driver,		/* (c) 1986 */
	/* MCR 68000 */
	&xenophob_driver,	/* (c) 1987 */
	/* Power Drive - similar to Xenophobe? */
/* other possible MCR games:
Black Belt
Shoot the Bull
Special Force
MotorDome
Six Flags (?)
*/

	/* Irem games */
	&skychut_driver,	/* (c) [1980] */
	&mpatrol_driver,	/* (c) 1982 */
	&mpatrolw_driver,	/* (c) 1982 + Williams license */
	&mranger_driver,	/* bootleg */
	&troangel_driver,	/* (c) 1983 */
	&yard_driver,		/* (c) 1983 */
	&vsyard_driver,		/* (c) 1983/1984 */
	&vsyard2_driver,	/* (c) 1983/1984 */
	&travrusa_driver,	/* (c) 1983 */
	&motorace_driver,	/* (c) 1983 Williams license */
	&kungfum_driver,	/* (c) 1984 */
	&kungfud_driver,	/* (c) 1984 + Data East license */
	&kungfub_driver,	/* bootleg */
	&kungfub2_driver,	/* bootleg */
	&battroad_driver,	/* (c) 1984 */
	&ldrun_driver,		/* (c) 1984 licensed from Broderbund */
	&ldruna_driver,		/* (c) 1984 licensed from Broderbund */
	&ldrun2_driver,		/* (c) 1984 licensed from Broderbund */
	&ldrun3_driver,		/* (c) 1985 licensed from Broderbund */
	&ldrun4_driver,		/* (c) 1986 licensed from Broderbund */
	&lotlot_driver,		/* (c) 1985 licensed from Tokuma Shoten */
	&kidniki_driver,	/* (c) 1986 + Data East USA license */
	&spelunk2_driver,	/* (c) 1986 licensed from Broderbund */
	&vigilant_driver,	/* (c) 1988 */
	&vigilntu_driver,	/* (c) 1988 */
	&vigilntj_driver,	/* (c) 1988 */

	/* Gottlieb/Mylstar games (Gottlieb became Mylstar in 1983) */
	&reactor_driver,	/* GV-100 (c) 1982 Gottlieb */
	&mplanets_driver,	/* GV-102 (c) 1983 Gottlieb */
	&qbert_driver,		/* GV-103 (c) 1982 Gottlieb */
	&qbertjp_driver,	/* GV-103 (c) 1982 Gottlieb + Konami license */
	&sqbert_driver,		/* (c) 1983 Mylstar - never released */
	&krull_driver,		/* GV-105 (c) 1983 Gottlieb */
	&mach3_driver,		/* GV-109 (c) 1983 Mylstar */
	&usvsthem_driver,	/* GV-??? (c) 198? Mylstar */
	&stooges_driver,	/* GV-113 (c) 1984 Mylstar */
	&qbertqub_driver,	/* GV-119 (c) 1983 Mylstar */
	&curvebal_driver,	/* GV-134 (c) 1984 Mylstar */

	/* older Taito games */
	&crbaloon_driver,	/* (c) 1980 Taito */
	&crbalon2_driver,	/* (c) 1980 Taito */

	/* Taito "Qix hardware" games */
	&qix_driver,		/* (c) 1981 Taito America */
	&qixa_driver,		/* (c) 1981 Taito America */
	&qix2_driver,		/* (c) 1981 Taito America */
	&sdungeon_driver,	/* (c) 1981 Taito America */
	&elecyoyo_driver,	/* (c) 1982 Taito America */
	&elecyoy2_driver,	/* (c) 1982 Taito America */
	&kram_driver,		/* (c) 1982 Taito America */
	&kram2_driver,		/* (c) 1982 Taito America */
	&zookeep_driver,	/* (c) 1982 Taito America */
	&zookeep2_driver,	/* (c) 1982 Taito America */
	&zookeep3_driver,	/* (c) 1982 Taito America */

	/* Taito SJ System games */
	&spaceskr_driver,	/* (c) 1981 */
	&junglek_driver,	/* (c) 1982 */
	&jhunt_driver,		/* (c) 1982 Taito America */
	&alpine_driver,		/* (c) 1982 */
	&alpinea_driver,	/* (c) 1982 */
	&timetunl_driver,	/* (c) 1982 */
	&wwestern_driver,	/* (c) 1982 */
	&frontlin_driver,	/* (c) 1982 */
	&elevator_driver,	/* (c) 1983 */
	&elevatob_driver,	/* bootleg */
	&tinstar_driver,	/* (c) 1983 */
	&waterski_driver,	/* (c) 1983 */
	&bioatack_driver,	/* (c) 1983 + Fox Video Games license */
	&sfposeid_driver,	/* (c) 1984 */

	/* other Taito games */
	/* NOTE: Taito games with "Taito Corporation Japan" copyright notice are *NOT* */
	/* Japanese versions (ie: rastan). Japanese versions have "Taito Corporation" only. */
	&bking2_driver,		/* (c) 1983 */
	&gsword_driver,		/* (c) 1984 */
	&retofinv_driver,	/* (c) 1985 */
	&retofin1_driver,	/* bootleg */
	&retofin2_driver,	/* bootleg */
	&gladiatr_driver,	/* (c) 1986 Taito America */
	&ogonsiro_driver,	/* (c) 1986 */
	&gcastle_driver,	/* (c) 1986 */
	&bublbobl_driver,	/* (c) 1986 */
	&bublbobr_driver,	/* (c) 1986 */
	&boblbobl_driver,	/* bootleg */
	&sboblbob_driver,	/* bootleg */
	&tokio_driver,		/* (c) 1986 */
	&tokiob_driver,		/* bootleg */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&mexico86_driver,
//	&kicknrun_driver,
	&rastan_driver,		/* (c) 1987 Taito Japan (World?) */
	&rastanu_driver,	/* (c) 1987 Taito America (US) */
	&rastanu2_driver,	/* (c) 1987 Taito America (US) */
	&rastsaga_driver,	/* (c) 1987 Taito (Japan)*/
	&rainbow_driver,	/* (c) 1987 */
	&rainbowe_driver,	/* (c) 1988 */
	&jumping_driver,	/* bootleg */
	&arkanoid_driver,	/* (c) 1986 Taito */
	&arknoidu_driver,	/* (c) 1986 Taito America + Romstar license */
	&arkbl2_driver,		/* bootleg */
	&arkatayt_driver,	/* bootleg */
	&superqix_driver,	/* (c) 1987 */
	&sqixbl_driver,		/* bootleg? but (c) 1987 */
	&twincobr_driver,	/* (c) 1987 */
	&twincobu_driver,	/* (c) 1987 Taito America + Romstar license */
	&ktiger_driver,		/* (c) 1987 */
	&extrmatn_driver,	/* (c) 1987 World Games */
	&arkanoi2_driver,	/* (c) 1987 Taito Japan (World?) */
	&ark2us_driver,		/* (c) 1987 + Romstar license */
	&tnzs_driver,		/* (c) 1988 Taito (Japan) */
	&tnzsb_driver,		/* bootleg but Taito Japan (World?) */
	&tnzs2_driver,		/* (c) 1988 Taito Japan (World?) */
	&insectx_driver,	/* (c) 1989 Taito Japan (World?) */
	&tigerh_driver,		/* (c) 1985 */
	&tigerh2_driver,	/* (c) 1985 */
	&tigerhb1_driver,	/* bootleg */
	&tigerhb2_driver,	/* bootleg */
	&slapfigh_driver,	/* (c) 1988 */
	&slapbtjp_driver,	/* bootleg */
	&slapbtuk_driver,	/* bootleg */
	&getstar_driver,	/* (c) 1986 Taito, but bootleg */
	&superman_driver,	/* (c) 1988 */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&lkage_driver,		/* (c) 1984 */

	/* Taito F2 games */
	&ssi_driver,		/* (c) 1990 */
	&majest12_driver,	/* (c) 1990 */
	&liquidk_driver,	/* (c) 1990 */
	&growl_driver,		/* (c) 1990 */

	/* Toaplan games */
	&truxton_driver,	/* (c) 1988 Taito */
	&zerowing_driver,	/* (c) 1989 Toaplan */
	&hellfire_driver,	/* (c) 1989 Toaplan + Taito license */
	&outzone_driver,	/* (c) 1989 Toaplan */
	&vimana_driver,		/* (c) 1991 Toaplan (+ Tecmo license when set to Japan) */
	&vimana2_driver,	/* (c) 1991 Toaplan (+ Tecmo license when set to Japan)  */
	&snowbros_driver,	/* (c) 1990 Toaplan + Romstar license */
	&snowbroa_driver,	/* (c) 1990 Toaplan + Romstar license */

	/* Williams games */
	&robotron_driver,	/* (c) 1982 */
	&robotryo_driver,	/* (c) 1982 */
	&stargate_driver,	/* (c) 1981 */
	&joust_driver,		/* (c) 1982 */
	&joustwr_driver,	/* (c) 1982 */
	&joustr_driver,		/* (c) 1982 */
	&sinistar_driver,	/* (c) 1982 */
	&sinista1_driver,	/* (c) 1982 */
	&sinista2_driver,	/* (c) 1982 */
	&bubbles_driver,	/* (c) 1982 */
	&bubblesr_driver,	/* (c) 1982 */
	&defender_driver,	/* (c) 1980 */
	&defendg_driver,	/* (c) 1980 */
	&splat_driver,		/* (c) 1982 */
	&blaster_driver,	/* (c) 1983 */
	&colony7_driver,	/* (c) 1981 Taito */
	&colony7a_driver,	/* (c) 1981 Taito */
	&lottofun_driver,	/* (c) 1987 H.A.R. Management */
	&defcmnd_driver,	/* bootleg */
	&defence_driver,	/* bootleg */
	&mysticm_driver,	/* (c) 1983 */
	&tshoot_driver,		/* (c) 1984 */
	&inferno_driver,	/* (c) 1984 */
	&joust2_driver,		/* (c) 1986 */

	/* Capcom games */
	/* The following is a COMPLETE list of the Capcom games up to 1997, as shown on */
	/* their web site. The list is sorted by production date. Some titles are in */
	/* quotes, because I couldn't find the English name (might not have been exported). */
	/* The name in quotes is the title Capcom used for the html page. */
	&vulgus_driver,		/*  5/1984 (c) 1984 */
	&vulgusj_driver,	/*  5/1984 (c) 1984 */
	&sonson_driver,		/*  7/1984 (c) 1984 */
	&higemaru_driver,	/*  9/1984 (c) 1984 */
	&c1942_driver,		/* 12/1984 (c) 1984 */
	&c1942a_driver,		/* 12/1984 (c) 1984 */
	&c1942b_driver,		/* 12/1984 (c) 1984 */
	&exedexes_driver,	/*  2/1985 (c) 1985 */
	&savgbees_driver,	/*  2/1985 (c) 1985 + Memetron license */
	&commando_driver,	/*  5/1985 (c) 1985 (World) */
	&commandu_driver,	/*  5/1985 (c) 1985 + Data East license (US) */
	&commandj_driver,	/*  5/1985 (c) 1985 (Japan) */
	&spaceinv_driver,	/* bootleg */
	&gng_driver,		/*  9/1985 (c) 1985 */
	&gngt_driver,		/*  9/1985 (c) 1985 */
	&gngcross_driver,	/*  9/1985 (c) 1985 */
	&gngjap_driver,		/*  9/1985 (c) 1985 */
	&diamond_driver,	/* (c) 1989 KH Video (NOT A CAPCOM GAME but runs on GnG hardware) */
	&gunsmoke_driver,	/* 11/1985 (c) 1985 (World) */
	&gunsmrom_driver,	/* 11/1985 (c) 1985 + Romstar (US) */
	&gunsmoka_driver,	/* 11/1985 (c) 1985 (US) */
	&gunsmokj_driver,	/* 11/1985 (c) 1985 (Japan) */
	&sectionz_driver,	/* 12/1985 (c) 1985 */
	&trojan_driver,		/*  4/1986 (c) 1986 + Romstar */
	&trojanj_driver,	/*  4/1986 (c) 1986 */
	&srumbler_driver,	/*  9/1986 (c) 1986 */	/* aka Rush'n Crash */
	&srumblr2_driver,	/*  9/1986 (c) 1986 */
	&lwings_driver,		/* 11/1986 (c) 1986 */
	&lwings2_driver,	/* 11/1986 (c) 1986 */
	&lwingsjp_driver,	/* 11/1986 (c) 1986 */
	&sidearms_driver,	/* 12/1986 (c) 1986 (World) */
	&sidearmr_driver,	/* 12/1986 (c) 1986 + Romstar license (US) */
	&sidearjp_driver,	/* 12/1986 (c) 1986 (Japan) */
	&turtship_driver,	/* (c) 1988 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	&dyger_driver,		/* (c) 1989 Philco (NOT A CAPCOM GAME but runs on modified Sidearms hardware) */
	&avengers_driver,	/*  2/1987 (c) 1987 (US) */
	&avenger2_driver,	/*  2/1987 (c) 1987 (US) */
	&bionicc_driver,	/*  3/1987 (c) 1987 (US) */
	&bionicc2_driver,	/*  3/1987 (c) 1987 (US) */
	&topsecrt_driver,	/*  3/1987 (c) 1987 (Japan) */
	&c1943_driver,		/*  6/1987 (c) 1987 (US) */
	&c1943jap_driver,	/*  6/1987 (c) 1987 (Japan) */
	&blktiger_driver,	/*  8/1987 (c) 1987 (US) */
	&bktigerb_driver,	/* bootleg */
	&blkdrgon_driver,	/*  8/1987 (c) 1987 (Japan) */
	&blkdrgnb_driver,	/* bootleg, hacked to say Black Tiger */
	&sf1_driver,		/*  8/1987 (c) 1987 (World) */
	&sf1us_driver,		/*  8/1987 (c) 1987 (US) */
	&sf1jp_driver,		/*  8/1987 (c) 1987 (Japan) */
	&tigeroad_driver,	/* 11/1987 (c) 1987 + Romstar (US) */
	&f1dream_driver,	/*  4/1988 (c) 1988 + Romstar */
	&f1dreamb_driver,	/* bootleg */
	&c1943kai_driver,	/*  6/1988 (c) 1987 (Japan) */
	&lastduel_driver,	/*  7/1988 (c) 1988 (US) */
	&lstduela_driver,	/*  7/1988 (c) 1988 (US) */
	&lstduelb_driver,	/* bootleg */
	&madgear_driver,	/*  2/1989 (c) 1989 (US) */
	&ledstorm_driver,	/*  2/1989 (c) 1989 (US) */
	/*  3/1989 Dokaben (baseball) */
	/*  8/1989 Dokaben 2 (baseball) */
	/* 10/1989 Capcom Baseball */
	/* 11/1989 Capcom World - see below among "Mitchell" games */
	/*  3/1990 Adventure Quiz 2 Hatena no Dai-Bouken - see below among "Mitchell" games */
	/*  1/1991 Quiz Tonosama no Yabou - see below among "Mitchell" games */
	/*  4/1991 "golf" */
	/*  5/1991 Ataxx - see below among "Leland" games */
	/*  6/1991 "sangoku" (quiz game) */
	/* 10/1991 Block Block - see below among "Mitchell" games */
	/*  6/1995 Street Fighter - the Movie - see below among "Incredible Technologies" games */
	/* 11/1995 Battle Arena Toshinden 2 (3D, not CPS) */
	/*  7/1996 Star Gladiator (3D, not CPS) */
	/* 12/1996 Street Fighter EX */
	/*  4/1997 Street Fighter EX Plus */

	/* Capcom CPS1 games */
	&forgottn_driver,	/*  7/1988 (c) 1988 (US) */	/* aka Lost Worlds */
	&ghouls_driver,		/* 12/1988 (c) 1988 */
	&ghoulsj_driver,	/* 12/1988 (c) 1988 */
	&strider_driver,	/*  3/1989 (c) 1989 */
	&striderj_driver,	/*  3/1989 (c) 1989 */
	&dwj_driver,		/*  4/1989 (c) 1989 */
	&willow_driver,		/*  6/1989 (c) 1989 (Japan) */
	&willowj_driver,	/*  6/1989 (c) 1989 (Japan) */
	&unsquad_driver,	/*  8/1989 (c) 1989 */
	&area88_driver,		/*  8/1989 (c) 1989 */
	&ffight_driver,		/* 12/1989 (c) (World) */
	&ffightu_driver,	/* 12/1989 (c) (US)    */
	&ffightj_driver,	/* 12/1989 (c) (Japan) */
	&c1941_driver,		/*  2/1990 (c) 1990 (World) */
	&c1941j_driver,		/*  2/1990 (c) 1990 (Japan) */
	&mercs_driver,		/*  3/ 2/1990 (c) 1990 (World) */
	&mercsu_driver,		/*  3/ 2/1990 (c) 1990 (US)    */
	&mercsj_driver,		/*  3/ 2/1990 (c) 1990 (Japan) */
	&mtwins_driver,		/*  6/19/1990 (c) 1990 (World) */
	&chikij_driver,		/*  6/19/1990 (c) 1990 (Japan) */
	&msword_driver,		/*  7/25/1990 (c) 1990 (World) */
	&mswordu_driver,	/*  7/25/1990 (c) 1990 (US)    */
	&mswordj_driver,	/*  6/23/1990 (c) 1990 (Japan) */
	&cawing_driver,		/* 10/12/1990 (c) 1990 (World) */
	&cawingj_driver,	/* 10/12/1990 (c) 1990 (Japan) */
	&nemo_driver,		/* 11/30/1990 (c) 1990 (World) */
	&nemoj_driver,		/* 11/20/1990 (c) 1990 (Japan) */
	&sf2_driver,		/*  2/14/1991 (c) 1991 (World) */
	&sf2a_driver,		/*  2/ 6/1991 (c) 1991 (US)    */
	&sf2b_driver,		/*  2/14/1991 (c) 1991 (US)    */
	&sf2e_driver,		/*  2/28/1991 (c) 1991 (US)    */
	&sf2j_driver,		/* 12/10/1991 (c) 1991 (Japan) */
	&c3wonders_driver,	/*  5/20/1991 (c) 1991 (US) */
	&c3wonderj_driver,	/*  5/20/1991 (c) 1991 (Japan) */
	&kod_driver,		/*  7/11/1991 (c) 1991 (World) */
	&kodj_driver,		/*  8/ 5/1991 (c) 1991 (Japan) */
	&kodb_driver,		/* bootleg */
	&captcomm_driver,	/* 10/14/1991 (c) 1991 (World) */
	&captcomu_driver,	/*  9/28/1991 (c) 1991 (US)    */
	&captcomj_driver,	/* 12/ 2/1991 (c) 1991 (Japan) */
	&knights_driver,	/* 11/27/1991 (c) 1991 (World) */
	&knightsj_driver,	/* 11/27/1991 (c) 1991 (Japan) */
	&sf2ce_driver,		/*  3/13/1992 (c) 1992 (World) */
	&sf2cea_driver,		/*  3/13/1992 (c) 1992 (US)    */
	&sf2ceb_driver,		/*  5/13/1992 (c) 1992 (US)    */
	&sf2cej_driver,		/*  5/13/1992 (c) 1992 (Japan) */
	&sf2red_driver,		/* hack */
//	&sf2rb_driver,		/* hack */
	&varth_driver,		/*  6/12/1992 (c) 1992 (World) */
	&varthj_driver,		/*  7/14/1992 (c) 1992 (Japan) */
	&cworld2j_driver,	/*  6/11/1992 (c) 1992 (Japan) */
	&wof_driver,		/* 10/ 2/1992 (c) 1992 (World) (CPS1 + QSound) */
	&wofj_driver,		/* 10/31/1992 (c) 1992 (Japan) (CPS1 + QSound) */
	&sf2t_driver,		/* 12/ 9/1992 (c) 1992 (US)    */
	&sf2tj_driver,		/* 12/ 9/1992 (c) 1992 (Japan) */
	&dino_driver,		/*  2/ 1/1993 (c) 1993 (World) (CPS1 + QSound) */
	&punisher_driver,	/*  4/22/1993 (c) 1993 (World) (CPS1 + QSound) */
	&punishrj_driver,	/*  4/22/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	&slammast_driver,	/*  7/13/1993 (c) 1993 (World) (CPS1 + QSound) */	/* aka Muscle Bomber */
	&mbomber_driver,	/* 12/ 6/1993 (c) 1993 (World) (CPS1 + QSound) */
	&mbomberj_driver,	/* 12/ 6/1993 (c) 1993 (Japan) (CPS1 + QSound) */
	&pnickj_driver,		/*  6/ 8/1994 (c) 1994 + Compile license (Japan) not listed on Capcom's site? */
	&qad_driver,		/*  7/ 1/1992 (c) 1992 (US)    */
	&qadj_driver,		/*  9/21/1994 (c) 1994 (Japan) */
	/*  1/1995: Quiz Tonosama no Yabou */
	&megaman_driver,	/* 10/ 6/1995 (c) 1995 (Asia)  */
	&rockmanj_driver,	/*  9/22/1995 (c) 1995 (Japan) */

	/* Capcom CPS2 games */
	/* list completed with Chris Mullins' FAQ */
	/* http://members.aol.com/CMull11217/private/index.htm */
	/* 10/1993 Super Street Fighter II */
	/*  1/1994 Dungeons & Dragons - Tower of Doom */
	/*  3/1994 Super Street Fighter II Turbo / Super Street Fighter II X */
	/*  5/1994 Alien vs Predator */
	/*  6/1994 Eco Fighters / Ultimate Ecology */
	/*  7/1994 Dark Stalkers / Vampire */
	/*  9/1994 Saturday Night Slam Masters 2 - Ring of Destruction / Super Muscle Bomber */
	/* 10/1994 Armored Warriors / Powered Gear */
	/* 12/1994 X-Men - Children of the Atom */
	/*  3/1995 Night Warriors - Dark Stalkers Revenge / Vampire Hunter */
	/*  4/1995 Cyberbots */
	/*  6/1995 Street Fighter Alpha / Street Fighter Zero */
	/* 11/1995 Marvel Super Heroes */
	/*  1/1996 19XX: The Battle Against Destiny */
	/*  2/1996 Dungeons & Dragons - Shadow Over Mystara */
	/*  3/1996 Street Fighter Alpha 2 / Street Fighter Zero 2 */
	/*  6/1996 Super Puzzle Fighter II / Turbo Super Puzzle Fighter II X */
	/*  7/1996 Rockman 2 - The Power Fighters */
	/*  8/1996 Street Fighter Zero 2 Alpha */
	/*  9/1996 Quiz Naneiro Dreams */
	/*  9/1996 X-Men vs. Street Fighter */
	/* 1996 Dark Stalkers 3 - Jedah's Damnation / Vampire Savior */
	/* 1997 Battle Circuit */
	/* 1997 Marvel Super Heroes vs Street Fighter */
	/* 1997 Super Gem Fighter / Pocket Fighter */
	/* 1997 Vampire Hunter 2 */
	/* 1997 Vampire Savior 2 */
	/* 1998 Marvel vs Capcom */
	/* 1998 Street Fighter Alpha 3 / Street Fighter Zero 3 */
	/* 1999 Giga Wing */
	/* another unlisted puzzle game: Gulum Pa! */

	/* Capcom CPS3 games */
	/* 10/1996 Warzard */
	/*  2/1997 Street Fighter III - New Generation */
	/* ???? Jojo's Bizarre Adventure */
	/* ???? Street Fighter 3: Second Impact ~giant attack~ */
	/* ???? Street Fighter 3: Third Strike ~fight to the finish~ */

	/* "Capcom Bowling hardware" games */
	&capbowl_driver,	/* (c) 1988 Incredible Technologies */
	&capbowl2_driver,	/* (c) 1988 Incredible Technologies */
	&clbowl_driver,		/* (c) 1989 Incredible Technologies */
	&bowlrama_driver,	/* (c) 1991 P & P Marketing */
/*
The Incredible Technologies game list
http://www.itsgames.com/it/CorporateProfile/corporateprofile_main.htm

ShuffleShot - (Incredible Technologies, Inc.)
Peter Jacobsen's Golden Tee '97 - (Incredible Technologies, Inc.)
World Class Bowling - (Incredible Technologies, Inc.)
Peter Jacobsen's Golden Tee 3D Golf - (Incredible Technologies, Inc.)
Street Fighter - "The Movie" (Capcom)
PAIRS - (Strata)
BloodStorm - (Strata)
Driver's Edge - (Strata)
NFL Hard Yardage - (Strata)
Time Killers - (Strata)
Neck 'n' Neck - (Bundra Games)
Ninja Clowns - (Strata)
Rim Rockin' Basketball - (Strata)
Arlington Horse Racing - (Strata)
Dyno Bop - (Grand Products)
Poker Dice - (Strata)
Peggle - (Strata)
Slick Shot - (Grand Products)
Golden Tee Golf II - (Strata)
Hot Shots Tennis - (Strata)
Strata Bowling - (Strata)
Golden Tee Golf I - (Strata)
Capcom Bowling - (Strata)
*/

	/* Mitchell games - only Block Block is listed on Capcom's site */
	&pang_driver,		/* (c) 1989 Mitchell (World)*/
	&pangb_driver,		/* bootleg */
	&bbros_driver,		/* (c) 1989 Capcom (US) */
	/* 11/1989 Capcom World */
	&hatena_driver,		/*  2/28/1990 (c) 1990 Capcom (Japan) */
	&spang_driver,		/*  9/14/1990 (c) 1990 Mitchell (World) */
	&sbbros_driver,		/* 10/ 1/1990 (c) 1990 Mitchell + Capcom (US) */
	&qtono1_driver,		/* 12/25/1990 (c) 1991 Capcom (Japan) */
	&block_driver,		/*  9/10/1991 (c) 1991 Capcom (World) */
	&blockbl_driver,	/* bootleg */

	/* Gremlin 8080 games */
	/* the numbers listed are the range of ROM part numbers */
	&blockade_driver,	/* 1-4 [1977 Gremlin] */
	&comotion_driver,	/* 5-7 [1977 Gremlin] */
	&hustle_driver,		/* 16-21 [1977 Gremlin] */
	&blasto_driver,		/* [1978 Gremlin] */

	/* Gremlin/Sega "VIC dual game board" games */
	/* the numbers listed are the range of ROM part numbers */
	&depthch_driver,	/* 50-55 [1977 Gremlin?] */
	&safari_driver,		/* 57-66 [1977 Gremlin?] */
	&frogs_driver,		/* 112-119 [1978 Gremlin?] */
	&sspaceat_driver,	/* 155-162 (c) */
	&sspacatc_driver,	/* 139-146 (c) */
	&headon_driver,		/* 163-167/192-193 (c) Gremlin */
	&headonb_driver,	/* 163-167/192-193 (c) Gremlin */
	&headon2_driver,	/* ???-??? (c) 1979 Sega */
	/* ???-??? Fortress */
	/* ???-??? Gee Bee */
	/* 255-270  Head On 2 / Deep Scan */
	&invho2_driver,		/* 271-286 (c) 1979 Sega */
	&samurai_driver,	/* 289-302 + upgrades (c) 1980 Sega */
	&invinco_driver,	/* 310-318 (c) 1979 Sega */
	&invds_driver,		/* 367-382 (c) 1979 Sega */
	&tranqgun_driver,	/* 413-428 (c) 1980 Sega */
	/* 450-465  Tranquilizer Gun (different version?) */
	/* ???-??? Car Hunt / Deep Scan */
	&spacetrk_driver,	/* 630-645 (c) 1980 Sega */
	&sptrekct_driver,	/* (c) 1980 Sega */
	&carnival_driver,	/* 651-666 (c) 1980 Sega */
	&carnvckt_driver,	/* 501-516 (c) 1980 Sega */
	&digger_driver,		/* 684-691 no copyright notice */
	&pulsar_driver,		/* 790-805 (c) 1981 Sega */
	&heiankyo_driver,	/* (c) [1979?] Denki Onkyo */

	/* Sega G-80 vector games */
	&spacfury_driver,	/* (c) 1981 */
	&spacfura_driver,	/* no copyright notice */
	&zektor_driver,		/* (c) 1982 */
	&tacscan_driver,	/* (c) */
	&elim2_driver,		/* (c) 1981 Gremlin */
	&elim2a_driver,		/* (c) 1981 Gremlin */
	&elim4_driver,		/* (c) 1981 Gremlin */
	&startrek_driver,	/* (c) 1982 */

	/* Sega G-80 raster games */
	&astrob_driver,		/* (c) 1981 */
	&astrob1_driver,	/* (c) 1981 */
	&s005_driver,		/* (c) 1981 */
	&monsterb_driver,	/* (c) 1982 */
	&spaceod_driver,	/* (c) 1981 */
	&pignewt_driver,	/* (c) 1983 */
	&pignewta_driver,	/* (c) 1983 */
	&sindbadm_driver,	/* 834-5244 (c) 1983 Sega */

	/* Sega "Zaxxon hardware" games */
	&zaxxon_driver,		/* (c) 1982 */
	&zaxxon2_driver,	/* (c) 1982 */
	&szaxxon_driver,	/* (c) 1982 */
	&futspy_driver,		/* (c) 1984 */
	&congo_driver,		/* 605-5167 (c) 1983 */
	&tiptop_driver,		/* 605-5167 (c) 1983 */

	/* Sega System 1 / System 2 games */
	&starjack_driver,	/* 834-5191 (c) 1983 (S1) */
	&starjacs_driver,	/* (c) 1983 Stern (S1) */
	&regulus_driver,	/* 834-5328 (c) 1983 (S1) */
	&regulusu_driver,	/* 834-5328 (c) 1983 (S1) */
	&upndown_driver,	/* (c) 1983 (S1) */
	&mrviking_driver,	/* 834-5383 (c) 1984 (S1) */
	&swat_driver,		/* 834-5388 (c) 1984 Coreland / Sega (S1) */
	&flicky_driver,		/* (c) 1984 (S1) */
	&flicky2_driver,	/* (c) 1984 (S1) */
	/* Water Match (S1) */
	&bullfgtj_driver,	/* 834-5478 (c) 1984 Sega / Coreland (S1) */
	&pitfall2_driver,	/* 834-5627 [1985?] reprogrammed, (c) 1984 Activision (S1) */
	&pitfallu_driver,	/* 834-5627 [1985?] reprogrammed, (c) 1984 Activision (S1) */
	&seganinj_driver,	/* 834-5677 (c) 1985 (S1) */
	&seganinu_driver,	/* 834-5677 (c) 1985 (S1) */
	&nprinces_driver,	/* 834-5677 (c) 1985 (S1) */
	&nprincsu_driver,	/* 834-5677 (c) 1985 (S1) */
	&nprincsb_driver,	/* bootleg? (S1) */
	&imsorry_driver,	/* 834-5707 (c) 1985 Coreland / Sega (S1) */
	&imsorryj_driver,	/* 834-5707 (c) 1985 Coreland / Sega (S1) */
	&teddybb_driver,	/* 834-5712 (c) 1985 (S1) */
	&hvymetal_driver,	/* 834-5745 (c) 1985 (S2?) */
	&myhero_driver,		/* 834-5755 (c) 1985 (S1) */
	&myheroj_driver,	/* 834-5755 (c) 1985 Coreland / Sega (S1) */
	&myherok_driver,	/* 834-5755 (c) 1985 Coreland / Sega (S1) */
	&shtngmst_driver,	/* 834-5719/5720 (c) 1985 (S2) */
	&chplft_driver,		/* 834-5795 (c) 1985, (c) 1982 Dan Gorlin (S2) */
	&chplftb_driver,	/* 834-5795 (c) 1985, (c) 1982 Dan Gorlin (S2) */
	&chplftbl_driver,	/* bootleg (S2) */
	&fdwarrio_driver,	/* 834-5918 (c) 1985 Coreland / Sega (S1) */
	&brain_driver,		/* (c) 1986 Coreland / Sega (S2?) */
	&wboy_driver,		/* 834-5984 (c) 1986 + Escape license (S1) */
	&wboy2_driver,		/* 834-5984 (c) 1986 + Escape license (S1) */
	&wboy3_driver,
	&wboy4_driver,		/* 834-5984 (c) 1986 + Escape license (S1) */
	&wboyu_driver,		/* 834-5753 (? maybe a conversion) (c) 1986 + Escape license (S1) */
	&wboy4u_driver,		/* 834-5984 (c) 1986 + Escape license (S1) */
	&wbdeluxe_driver,	/* (c) 1986 + Escape license (S1) */
	&gardia_driver,		/* 834-6119 (S2?) */
	&blockgal_driver,	/* 834-6303 (S1) */
	&tokisens_driver,	/* (c) 1987 (from a bootleg board) (S2) */
	&dakkochn_driver,	/* 836-6483? (S2) */
	&ufosensi_driver,	/* 834-6659 (S2) */
	&wbml_driver,		/* bootleg (S2?) */
/*
other System 1 / System 2 games:

WarBall
Rafflesia
Sanrin Sanchan
DokiDoki Penguin Land *not confirmed
*/

	/* other Sega 8-bit games */
	&turbo_driver,		/* (c) 1981 Sega */
	&turboa_driver,		/* (c) 1981 Sega */
	&turbob_driver,		/* (c) 1981 Sega */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&kopunch_driver,	/* 834-0103 (c) 1981 Sega */
	&suprloco_driver,	/* (c) 1982 Sega */
	&champbas_driver,	/* (c) 1983 Sega */
	&champbb2_driver,
	&bankp_driver,		/* (c) 1984 Sega */

	/* Sega System 16 games */
	&alexkidd_driver,	/* (c) 1986 (but bootleg) */
	&aliensyn_driver,	/* (c) 1987 */
	&altbeast_driver,	/* (c) 1988 */
	&astormbl_driver,
	&aurail_driver,
	&dduxbl_driver,
	&eswatbl_driver,	/* (c) 1989 (but bootleg) */
	&fantzone_driver,
	&fpointbl_driver,
	&goldnaxe_driver,	/* (c) 1989 */
	&hwchamp_driver,
	&mjleague_driver,	/* (c) 1985 */
	&passshtb_driver,	/* bootleg */
	&quartet2_driver,
	&sdi_driver,		/* (c) 1987 */
	&shinobi_driver,	/* (c) 1987 */
	&tetrisbl_driver,	/* (c) 1988 (but bootleg) */
	&timscanr_driver,
	&tturfbl_driver,
	&wb3bl_driver,
	&wrestwar_driver,	/* (c) 1989 */

	/* Data East "Burger Time hardware" games */
	&lnc_driver,		/* (c) 1981 */
	&zoar_driver,		/* (c) 1982 */
	&btime_driver,		/* (c) 1982 + Midway */
	&btimed_driver,		/* (c) 1982 */
	&btimed2_driver,	/* (c) 1982 */
	&wtennis_driver,	/* bootleg 1982 */
	&bnj_driver,		/* (c) 1982 + Midway */
	&brubber_driver,	/* (c) 1982 */
	&caractn_driver,	/* bootleg */
	&disco_driver,		/* (c) 1982 */
	&mmonkey_driver,	/* (c) 1982 Technos Japan + Roller Tron */
	&eggs_driver,		/* (c) 1983 Universal USA */
	&scregg_driver,		/* TA-0001 (c) 1983 Technos Japan */
	&tagteam_driver,	/* TA-0007 (c) 1983 + Technos Japan license */
	/* cassette system */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&decocass_driver,
	&cookrace_driver,	/* bootleg */

	/* other Data East games */
	&astrof_driver,		/* (c) [1980?] */
	&astrof2_driver,	/* (c) [1980?] */
	&astrof3_driver,	/* (c) [1980?] */
	&tomahawk_driver,	/* (c) [1980?] */
	&tomahaw5_driver,	/* (c) [1980?] */
	&kchamp_driver,		/* (c) 1984 Data East USA (US) */
	&kchampvs_driver,	/* (c) 1984 Data East USA (US) */
	&karatedo_driver,	/* (c) 1984 Data East Corporation (Japan) */
	&firetrap_driver,	/* (c) 1986 */
	&firetpbl_driver,	/* bootleg */
	&brkthru_driver,	/* (c) 1986 Data East USA (US) */
	&brkthruj_driver,	/* (c) 1986 Data East Corporation (Japan) */
	&darwin_driver,		/* (c) 1986 Data East Corporation (Japan) */
	&shootout_driver,	/* (c) 1985 Data East USA (US) */
	&sidepckt_driver,	/* (c) 1986 Data East Corporation (World?) */
	&exprraid_driver,	/* (c) 1986 Data East USA (US) */
	&wexpress_driver,	/* (c) 1986 Data East Corporation (World?) */
	&wexpresb_driver,	/* bootleg */
	&pcktgal_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&pcktgalb_driver,	/* bootleg */
	&pcktgal2_driver,	/* (c) 1989 Data East Corporation (World?) */
	&spool3_driver,		/* (c) 1989 Data East Corporation (World?) */
	&spool3i_driver,	/* (c) 1990 Data East Corporation + I-Vics license */
	&actfancr_driver,	/* (c) 1989 Data East Corporation */

	/* Data East 8-bit games */
	&lastmiss_driver,	/* (c) 1986 Data East USA (US) */
	&lastmss2_driver,	/* (c) 1986 Data East USA (US) */
	&shackled_driver,	/* (c) 1986 Data East USA (US) */
	&breywood_driver,	/* (c) 1986 Data East Corporation (Japan) */
	&csilver_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&ghostb_driver,		/* (c) 1987 Data East USA (US) */
	&ghostb3_driver,	/* (c) 1987 Data East USA (US) */
	&mazeh_driver,		/* (c) 1987 Data East Corporation (Japan) */
	&srdarwin_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&gondo_driver,		/* (c) 1987 Data East USA (US) */
	&mekyosen_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&cobracom_driver,	/* (c) 1988 Data East Corporation (Japan) */
	&oscar_driver,		/* (c) 1988 Data East USA (US) */
	&oscarj_driver,		/* (c) 1987 Data East Corporation (Japan) */

	/* Data East 16-bit games */
	&karnov_driver,		/* (c) 1987 Data East USA (US) */
	&karnovj_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&chelnov_driver,	/* (c) 1988 Data East USA (US) */
	&chelnovj_driver,	/* (c) 1988 Data East Corporation (Japan) */
	/* the following ones all run on similar hardware */
	&hbarrel_driver,	/* (c) 1987 Data East USA (US) */
	&hbarrelj_driver,	/* (c) 1987 Data East Corporation (Japan) */
	&baddudes_driver,	/* (c) 1988 Data East USA (US) */
	&drgninja_driver,	/* (c) 1988 Data East Corporation (Japan) */
	&robocopp_driver,	/* bootleg */
	&hippodrm_driver,	/* (c) 1989 Data East USA (US) */
	&ffantasy_driver,	/* (c) 1989 Data East Corporation (Japan) */
	&midres_driver,		/* (c) 1989 Data East USA (US) */
	&midresj_driver,	/* (c) 1989 Data East Corporation (Japan) */
	&slyspy_driver,		/* (c) 1989 Data East USA (US) */
	&slyspy2_driver,	/* (c) 1989 Data East USA (US) */
	&bouldash_driver,	/* (c) 1990 Data East Corporation */
	/* evolution of the hardware */
	&darkseal_driver,	/* (c) 1990 Data East Corporation (World) */
	&darksea1_driver,	/* (c) 1990 Data East Corporation (World) */
	&gatedoom_driver,	/* (c) 1990 Data East Corporation (US) */
	&gatedom1_driver,	/* (c) 1990 Data East Corporation (US) */
	&supbtime_driver,	/* (c) 1990 Data East Corporation (Japan) */
	&cninja_driver,		/* (c) 1991 Data East Corporation (World) */
	&cninja0_driver,	/* (c) 1991 Data East Corporation (World) */
	&cninjau_driver,	/* (c) 1991 Data East Corporation (US) */
	&joemac_driver,		/* (c) 1991 Data East Corporation (Japan) */
	&stoneage_driver,	/* bootleg */
	&tumblep_driver,	/* 1991 bootleg */
	&tumblep2_driver,	/* 1991 bootleg */

	/* Tehkan / Tecmo games (Tehkan became Tecmo in 1986) */
	&senjyo_driver,		/* (c) 1983 Tehkan */
	&starforc_driver,	/* (c) 1984 Tehkan */
	&starfore_driver,	/* (c) 1984 Tehkan */
	&megaforc_driver,	/* (c) 1985 Tehkan + Video Ware license */
	&bombjack_driver,	/* (c) 1984 Tehkan */
	&bombjac2_driver,	/* (c) 1984 Tehkan */
	&pbaction_driver,	/* (c) 1985 Tehkan */
	&pbactio2_driver,	/* (c) 1985 Tehkan */
	&pontoon_driver,	/* 6011 - (c) 1985 Tehkan */
	&tehkanwc_driver,	/* (c) 1985 Tehkan */
	&gridiron_driver,	/* (c) 1985 Tehkan */
	&teedoff_driver,	/* 6102 - (c) 1986 Tecmo */
	&solomon_driver,	/* (c) 1986 Tecmo */
	&rygar_driver,		/* 6002 - (c) 1986 Tecmo */
	&rygar2_driver,		/* 6002 - (c) 1986 Tecmo */
	&rygarj_driver,		/* 6002 - (c) 1986 Tecmo */
	&gemini_driver,		/* (c) 1987 Tecmo */
	&silkworm_driver,	/* 6217 - (c) 1988 Tecmo */
	&silkwrm2_driver,	/* 6217 - (c) 1988 Tecmo */
	&gaiden_driver,		/* 6215 - (c) 1988 Tecmo */
	&shadoww_driver,	/* 6215 - (c) 1988 Tecmo */
	&tknight_driver,	/* (c) 1989 Tecmo */
	&wildfang_driver,	/* (c) 1989 Tecmo */
	&wc90_driver,		/* (c) 1989 Tecmo */
	&wc90b_driver,		/* bootleg */

	/* Konami bitmap games */
	&tutankhm_driver,	/* GX350 (c) 1982 Konami */
	&tutankst_driver,	/* GX350 (c) 1982 Stern */
	&junofrst_driver,	/* GX310 (c) 1983 Konami */

	/* Konami games */
	&pooyan_driver,		/* GX320 (c) 1982 */
	&pooyans_driver,	/* GX320 (c) 1982 Stern */
	&pootan_driver,		/* bootleg */
	&timeplt_driver,	/* GX393 (c) 1982 */
	&timepltc_driver,	/* GX393 (c) 1982 + Centuri license*/
	&spaceplt_driver,	/* bootleg */
	&megazone_driver,	/* GX319 (c) 1983 + Kosuka */
	&rocnrope_driver,	/* GX364 (c) 1983 + Kosuka */
	&ropeman_driver,	/* bootleg */
	&gyruss_driver,		/* GX347 (c) 1983 */
	&gyrussce_driver,	/* GX347 (c) 1983 + Centuri license */
	&venus_driver,		/* bootleg */
	&trackfld_driver,	/* GX361 (c) 1983 */
	&trackflc_driver,	/* GX361 (c) 1983 + Centuri license */
	&hyprolym_driver,	/* GX361 (c) 1983 */
	&hyprolyb_driver,	/* bootleg */
	&circusc_driver,	/* GX380 (c) 1984 */
	&circusc2_driver,	/* GX380 (c) 1984 */
	&circuscc_driver,	/* GX380 (c) 1984 + Centuri license */
	&tp84_driver,		/* GX388 (c) 1984 */
	&tp84a_driver,		/* GX388 (c) 1984 */
	&hyperspt_driver,	/* GX330 (c) 1984 + Centuri */
	&sbasketb_driver,	/* GX405 (c) 1984 */
	&mikie_driver,		/* GX469 (c) 1984 */
	&mikiej_driver,		/* GX469 (c) 1984 */
	&mikiehs_driver,	/* GX469 (c) 1984 */
	&roadf_driver,		/* GX461 (c) 1984 */
	&roadf2_driver,		/* GX461 (c) 1984 */
	&yiear_driver,		/* GX407 (c) 1985 */
	&yiear2_driver,		/* GX407 (c) 1985 */
	&kicker_driver,		/* GX477 (c) 1985 */
	&shaolins_driver,	/* GX477 (c) 1985 */
	&pingpong_driver,	/* GX555 (c) 1985 */
	&gberet_driver,		/* GX577 (c) 1985 */
	&rushatck_driver,	/* GX577 (c) 1985 */
	&gberetb_driver,	/* bootleg on different hardware */
	&jailbrek_driver,	/* GX507 (c) 1986 */
	&ironhors_driver,	/* GX560 (c) 1986 */
	&farwest_driver,
	&jackal_driver,		/* GX631 (c) 1986 */
	&topgunr_driver,	/* GX631 (c) 1986 */
	&topgunbl_driver,	/* bootleg */
	&ddrible_driver,	/* GX690 (c) 1986 */
	&contra_driver,		/* GX633 (c) 1987 */
	&contrab_driver,	/* bootleg */
	&gryzorb_driver,	/* bootleg */
	&mainevt_driver,	/* GX799 (c) 1988 */
	&mainevt2_driver,	/* GX799 (c) 1988 */
	&devstors_driver,	/* GX890 (c) 1988 */
	&combasc_driver,	/* GX611 (c) 1988 */
	&combascb_driver,	/* bootleg */

	/* Konami "Nemesis hardware" games */
	&nemesis_driver,	/* GX456 (c) 1985 */
	&nemesuk_driver,	/* GX456 (c) 1985 */
	&konamigt_driver,	/* GX561 (c) 1985 */
	&salamand_driver,	/* GX587 (c) 1986 */
	&lifefrce_driver,	/* GX587 (c) 1986 */
	&lifefrcj_driver,	/* GX587 (c) 1986 */
	/* GX400 BIOS based games */
	&rf2_driver,		/* GX561 (c) 1985 */
	&twinbee_driver,	/* GX412 (c) 1985 */
	&gradius_driver,	/* GX456 (c) 1985 */
	&gwarrior_driver,	/* GX578 (c) 1985 */

	/* Konami "TMNT hardware" games */
	&tmnt_driver,		/* GX963 (c) 1989 */
	&tmntj_driver,		/* GX963 (c) 1989 */
	&tmht2p_driver,		/* GX963 (c) 1989 */
	&tmnt2pj_driver,	/* GX963 (c) 1990 */
	&punkshot_driver,	/* GX907 (c) 1990 */
	&punksht2_driver,	/* GX907 (c) 1990 */

	/* Exidy games */
	&sidetrac_driver,	/* (c) 1979 */
	&targ_driver,		/* (c) 1980 */
	&spectar_driver,	/* (c) 1980 */
	&spectar1_driver,	/* (c) 1980 */
	&venture_driver,	/* (c) 1981 */
	&venture2_driver,	/* (c) 1981 */
	&venture4_driver,	/* (c) 1981 */
	&mtrap_driver,		/* (c) 1981 */
	&mtrap3_driver,		/* (c) 1981 */
	&mtrap4_driver,		/* (c) 1981 */
	&pepper2_driver,	/* (c) 1982 */
	&hardhat_driver,	/* (c) 1982 */
	&fax_driver,		/* (c) 1983 */
	&circus_driver,		/* no copyright notice [1977?] */
	&robotbwl_driver,	/* no copyright notice */
	&crash_driver,		/* Exidy [1979?] */
	&ripcord_driver,	/* Exidy [1977?] */
	&starfire_driver,	/* Exidy [1979?] */
	&fireone_driver,	/* (c) 1979 Exidy */

	/* Exidy 440 games */
	&crossbow_driver,	/* (c) 1983 */
	&cheyenne_driver,	/* (c) 1984 */
	&combat_driver,		/* (c) 1985 */
	&cracksht_driver,	/* (c) 1985 */
	&claypign_driver,	/* (c) 1986 */
	&chiller_driver,	/* (c) 1986 */
	&topsecex_driver,	/* (c) 1986 */
	&hitnmiss_driver,	/* (c) 1987 */
	&hitnmis2_driver,	/* (c) 1987 */
	&whodunit_driver,	/* (c) 1988 */
	&showdown_driver,	/* (c) 1988 */

	/* Atari vector games */
	&asteroid_driver,	/* (c) 1979 */
	&asteroi1_driver,	/* no copyright notice */
	&astdelux_driver,	/* (c) 1980 */
	&astdelu1_driver,	/* (c) 1980 */
	&bwidow_driver,		/* (c) 1982 */
	&bzone_driver,		/* (c) 1980 */
	&bzone2_driver,		/* (c) 1980 */
	&gravitar_driver,	/* (c) 1982 */
	&gravitr2_driver,	/* (c) 1982 */
	&llander_driver,	/* no copyright notice */
	&llander1_driver,	/* no copyright notice */
	&redbaron_driver,	/* (c) 1980 */
	&spacduel_driver,	/* (c) 1980 */
	&tempest_driver,	/* (c) 1980 */
	&tempest1_driver,	/* (c) 1980 */
	&tempest2_driver,	/* (c) 1980 */
	&temptube_driver,	/* hack */
	&starwars_driver,	/* (c) 1983 */
	&starwar1_driver,	/* (c) 1983 */
	&empire_driver,		/* (c) 1985 */
	&mhavoc_driver,		/* (c) 1983 */
	&mhavoc2_driver,	/* (c) 1983 */
	&mhavocrv_driver,	/* hack */
	&quantum_driver,	/* (c) 1982 */	/* made by Gencomp */
	&quantum1_driver,	/* (c) 1982 */	/* made by Gencomp */
	&quantump_driver,	/* (c) 1982 */	/* made by Gencomp */

	/* Atari "Centipede hardware" games */
	&warlord_driver,	/* (c) 1980 */
	&centiped_driver,	/* (c) 1980 */
	&centipd2_driver,	/* (c) 1980 */
	&centipdb_driver,	/* bootleg */
	&milliped_driver,	/* (c) 1982 */
	&qwakprot_driver,	/* (c) 1982 */

	/* Atari "Kangaroo hardware" games */
	&kangaroo_driver,	/* (c) 1982 */
	&kangarob_driver,	/* bootleg */
	&arabian_driver,	/* (c) 1983 Sun Electronics */
	&arabiana_driver,	/* (c) 1983 */

	/* Atari "Missile Command hardware" games */
	&missile_driver,	/* (c) 1980 */
	&missile2_driver,	/* (c) 1980 */
	&suprmatk_driver,	/* (c) 1980 + (c) 1981 Gencomp */

	/* Atari b/w games */
	&sprint1_driver,	/* no copyright notice */
	&sprint2_driver,	/* no copyright notice */
	&sbrkout_driver,	/* no copyright notice */
	&dominos_driver,	/* no copyright notice */
	&nitedrvr_driver,	/* no copyright notice [1976] */
	&bsktball_driver,	/* no copyright notice */
	&copsnrob_driver,	/* [1976] */
	&avalnche_driver,	/* no copyright notice [1978] */
	&subs_driver,		/* no copyright notice [1976] */
	&atarifb_driver,	/* no copyright notice [1978] */
	&atarifb1_driver,	/* no copyright notice [1978] */
	&atarifb4_driver,	/* no copyright notice [1979] */
	&abaseb_driver,		/* no copyright notice [1979] */
	&abaseb2_driver,	/* no copyright notice [1979] */
	&canyon_driver,		/* no copyright notice [1977] */
	&canbprot_driver,	/* no copyright notice [1977] */
	&skydiver_driver,	/* no copyright notice [1977] */

	/* misc Atari games */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&polepos_driver,
	&foodf_driver,		/* (c) 1982 */	/* made by Gencomp */
	&liberatr_driver,	/* (c) 1982 */
	&ccastles_driver,	/* (c) 1983 */
	&ccastle2_driver,	/* (c) 1983 */
	&cloak_driver,		/* (c) 1983 */
	&cloud9_driver,		/* (c) 1983 */
	&jedi_driver,		/* (c) 1984 */

	/* Atari System 1 games */
	&marble_driver,		/* (c) 1984 */
	&marble2_driver,	/* (c) 1984 */
	&marblea_driver,	/* (c) 1984 */
	&peterpak_driver,	/* (c) 1984 */
	&indytemp_driver,	/* (c) 1985 */
	&indytem2_driver,	/* (c) 1985 */
	&roadrunn_driver,	/* (c) 1985 */
	&roadblst_driver,	/* (c) 1986, 1987 */

	/* Atari System 2 games */
	&paperboy_driver,	/* (c) 1984 */
	&ssprint_driver,	/* (c) 1986 */
	&csprint_driver,	/* (c) 1986 */
	&a720_driver,		/* (c) 1986 */
	&a720b_driver,		/* (c) 1986 */
	&apb_driver,		/* (c) 1987 */
	&apb2_driver,		/* (c) 1987 */

	/* later Atari games */
	&gauntlet_driver,	/* (c) 1985 */
	&gauntir1_driver,	/* (c) 1985 */
	&gauntir2_driver,	/* (c) 1985 */
	&gaunt2p_driver,	/* (c) 1985 */
	&gaunt2_driver,		/* (c) 1986 */
	&vindctr2_driver,	/* (c) 1988 */
	&atetris_driver,	/* (c) 1988 */
	&atetrisa_driver,	/* (c) 1988 */
	&atetrisb_driver,	/* bootleg */
	&atetcktl_driver,	/* (c) 1989 */
	&atetckt2_driver,	/* (c) 1989 */
	&toobin_driver,		/* (c) 1988 */
	&vindictr_driver,	/* (c) 1988 */
	&klax_driver,		/* (c) 1989 */
	&klax2_driver,		/* (c) 1989 */
	&klax3_driver,		/* (c) 1989 */
	&blstroid_driver,	/* (c) 1987 */
	&blstroi2_driver,	/* (c) 1987 */
	&xybots_driver,		/* (c) 1987 */
	&eprom_driver,		/* (c) 1989 */
	&eprom2_driver,		/* (c) 1989 */
	&skullxbo_driver,	/* (c) 1989 */
	&skullxb2_driver,	/* (c) 1989 */
	&badlands_driver,	/* (c) 1989 */
	&cyberb2p_driver,	/* (c) 1989 */
	&rampart_driver,	/* (c) 1990 */
	&ramprt2p_driver,	/* (c) 1990 */
	&shuuz_driver,		/* (c) 1990 */
	&shuuz2_driver,		/* (c) 1990 */

	/* SNK / Rock-ola games */
	&sasuke_driver,		/* [1980] Shin Nihon Kikaku (SNK) */
	&satansat_driver,	/* (c) 1981 SNK */
	&zarzon_driver,		/* (c) 1981 Taito, gameplay says SNK */
	&vanguard_driver,	/* (c) 1981 SNK */
	&vangrdce_driver,	/* (c) 1981 SNK + Centuri */
	&fantasy_driver,	/* (c) 1981 Rock-ola */
	&pballoon_driver,	/* (c) 1982 SNK */
	&nibbler_driver,	/* (c) 1982 Rock-ola */
	&nibblera_driver,	/* (c) 1982 Rock-ola */

	/* later SNK games */
	&tnk3_driver,		/* (c) 1985 */
	&tnk3j_driver,		/* (c) 1985 */
	&aso_driver,		/* (c) 1985 */
	&athena_driver	,	/* (c) 1986 */
	&fitegolf_driver,	/* (c) 1988 */
	&ikari_driver,		/* (c) 1986 */
	&ikarijp_driver,	/* (c) 1986 */
	&ikarijpb_driver,	/* bootleg */
	&victroad_driver,	/* (c) 1986 */
	&dogosoke_driver,	/* (c) 1986 */
	&gwar_driver,		/* (c) 1987 */
	&bermudat_driver,	/* (c) 1987 */
	&psychos_driver,	/* (c) 1987 */
	&psychosa_driver,	/* (c) 1987 */
	&chopper_driver,	/* (c) 1988 */
	&legofair_driver,	/* (c) 1988 */
	&tdfever_driver,	/* (c) 1987 */
	&tdfeverj_driver,	/* (c) 1987 */
	&pow_driver,		/* (c) 1988 */
	&powj_driver,		/* (c) 1988 */
	&prehisle_driver,	/* (c) 1989 */
	&prehislu_driver,	/* (c) 1989 */
	&prehislj_driver,	/* (c) 1989 */

	/* Technos games */
	&mystston_driver,	/* TA-0010 (c) 1984 */
	&matmania_driver,	/* TA-0015 (c) 1985 + Taito America license */
	&excthour_driver,	/* TA-0015 (c) 1985 + Taito license */
	&maniach_driver,	/* TA-???? (c) 1986 + Taito America license */
	&maniach2_driver,	/* TA-???? (c) 1986 + Taito America license */
	&renegade_driver,	/* TA-0018 (c) 1986 + Taito America license */
	&kuniokub_driver,	/* TA-0018 bootleg */
	&xsleena_driver,	/* TA-0019 (c) 1986 */
	&xsleenab_driver,	/* bootleg */
	&solarwar_driver,	/* TA-0019 (c) 1986 Taito + Memetron license */
	&battlane_driver,	/* TA-???? (c) 1986 + Taito license */
	&battlan2_driver,	/* TA-???? (c) 1986 + Taito license */
	&battlan3_driver,	/* TA-???? (c) 1986 + Taito license */
	&ddragon_driver,
	&ddragonb_driver,	/* TA-0021 bootleg */
	/* TA-0022 Super Dodge Ball */
	/* TA-0023 China Gate */
	/* TA-0024 WWF Superstars */
	/* TA-0025 Champ V'Ball */
	&ddragon2_driver,	/* TA-0026 (c) 1988 */
	/* TA-0028 Combatribes */
	&blockout_driver,	/* TA-0029 (c) 1989 + California Dreams */
	&blckout2_driver,	/* TA-0029 (c) 1989 + California Dreams */
	/* TA-0030 Double Dragon 3 */
	/* TA-0031 WWF Wrestlefest */

	/* Stern "Berzerk hardware" games */
	&berzerk_driver,	/* (c) 1980 */
	&berzerk1_driver,	/* (c) 1980 */
	&frenzy_driver,		/* (c) 1982 */

	/* GamePlan games */
	&megatack_driver,	/* (c) 1980 Centuri */
	&killcom_driver,	/* (c) 1980 Centuri */
	&challeng_driver,	/* (c) 1981 Centuri */
	&kaos_driver,		/* (c) 1981 */

	/* "stratovox hardware" games */
	&route16_driver,	/* (c) 1981 Leisure and Allied (bootleg) */
	&stratvox_driver,	/* Taito */
	&stratvxb_driver,	/* bootleg */
	&speakres_driver,	/* no copyright notice */

	/* Zaccaria games */
	&monymony_driver,	/* (c) 1983 */
	&jackrabt_driver,	/* (c) 1984 */
	&jackrab2_driver,	/* (c) 1984 */
	&jackrabs_driver,	/* (c) 1984 */

	/* UPL games */
	&nova2001_driver,	/* (c) [1984?] + Universal license */
	&pkunwar_driver,	/* [1985?] */
	&pkunwarj_driver,	/* [1985?] */
	&ninjakd2_driver,	/* (c) 1987 */
	&ninjak2a_driver,	/* (c) 1987 */
	&ninjak2b_driver,	/* (c) 1987 */
	&rdaction_driver,	/* (c) 1987 + World Games license */
	&mnight_driver,		/* (c) 1987 distributed by Kawakus */
/*
other UPL games:

   83 Mouser                              Kit 2P              Action
8/87 Mission XX                          Kit 2P  8W+2B   VC  Shooter
   88 Aquaria                             Kit 2P  8W+2B
   89 Ochichi Mahjong                     Kit 2P  8W+2B   HC  Mahjong
9/89 Omega Fighter        American Sammy Kit 2P  8W+2B   HC  Shooter
12/89 Task Force Harrier   American Sammy Kit 2P  8W+2B   VC  Shooter
   90 Atomic Robo-Kid      American Sammy Kit 2P  8W+2B   HC  Shooter
   90 Mustang - U.S.A.A.F./Fire Mustang   Kit 2P  8W+2B   HC  Shooter
   91 Acrobat Mission               Taito Kit 2P  8W+2B   VC  Shooter
   91 Bio Ship Paladin/Spaceship Gomera   Kit 2P  8W+2B   HC  Shooter
   91 Black Heart                         Kit 2P  8W+2B   HC  Shooter
   91 Spaceship Gomera/Bio Ship Paladin   Kit 2P  8W+2B   HC  Shooter
   91 Van Dyke Fantasy                    Kit 2P  8W+2B
2/92 Strahl                              Kit 2P  8W+3B

board numbers:
Atomic Robo Kid     UPL-88013
Omega Fighter       UPL-89016
Task Force Harrier  UPL-89050
USAAF Mustang       UPL-90058
Bio Ship Paladin    UPL-90062

*/

	/* Williams/Midway TMS34010 games */
	&narc_driver,		/* (c) 1988 Williams */
	&trog_driver,		/* (c) 1990 Midway */
	&trog3_driver,		/* (c) 1990 Midway */
	&trogp_driver,		/* (c) 1990 Midway */
	&smashtv_driver,	/* (c) 1990 Williams */
	&smashtv6_driver,	/* (c) 1990 Williams */
	&smashtv5_driver,	/* (c) 1990 Williams */
	&smashtv4_driver,	/* (c) 1990 Williams */
	&hiimpact_driver,	/* (c) 1990 Williams */
	&shimpact_driver,	/* (c) 1991 Midway */
	&strkforc_driver,	/* (c) 1991 Midway */
	&mk_driver,			/* (c) 1992 Midway */
	&mkla1_driver,		/* (c) 1992 Midway */
	&mkla2_driver,		/* (c) 1992 Midway */
	&term2_driver,		/* (c) 1992 Midway */
	&totcarn_driver,	/* (c) 1992 Midway */
	&totcarnp_driver,	/* (c) 1992 Midway */
	&mk2_driver,		/* (c) 1993 Midway */
	&mk2r32_driver,		/* (c) 1993 Midway */
	&mk2r14_driver,		/* (c) 1993 Midway */
	&nbajam_driver,		/* (c) 1993 Midway */

	/* Cinematronics raster games */
	&jack_driver,		/* (c) 1982 Cinematronics */
	&jack2_driver,		/* (c) 1982 Cinematronics */
	&jack3_driver,		/* (c) 1982 Cinematronics */
	&treahunt_driver,	/* (c) 1982 Hara Ind. */
	&zzyzzyxx_driver,	/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	&zzyzzyx2_driver,	/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	&brix_driver,		/* (c) 1982 Cinematronics + Advanced Microcomputer Systems */
	&sucasino_driver,	/* (c) 1982 Data Amusement */

	/* Cinematronics vector games */
	&spacewar_driver,
	&barrier_driver,
	&starcas_driver,
	&tgunner_driver,
	&ripoff_driver,
	&armora_driver,
	&wotw_driver,
	&warrior_driver,
	&starhawk_driver,
	&solarq_driver,
	&boxingb_driver,
	&speedfrk_driver,
	&sundance_driver,
	&demon_driver,

	/* "The Pit hardware" games */
	&roundup_driver,	/* (c) 1981 Amenip/Centuri */
	&fitter_driver,		/* (c) 1981 Taito */
	&thepit_driver,		/* (c) 1982 Centuri */
	&intrepid_driver,	/* (c) 1983 Nova Games Ltd. */
	&intrepi2_driver,	/* (c) 1983 Nova Games Ltd. */
	&portman_driver,	/* (c) 1982 Nova Games Ltd. */
	&suprmous_driver,	/* (c) 1982 Taito */
	&suprmou2_driver,	/* (c) 1982 Chu Co. Ltd. */

	/* Valadon Automation games */
	&bagman_driver,		/* (c) 1982 */
	&bagnard_driver,	/* (c) 1982 */
	&bagmans_driver,	/* (c) 1982 + Stern license */
	&bagmans2_driver,	/* (c) 1982 + Stern license */
	&sbagman_driver,	/* (c) 1984 */
	&sbagmans_driver,	/* (c) 1984 + Stern license */
	&pickin_driver,		/* (c) 1983 */

	/* Seibu Denshi / Seibu Kaihatsu games */
	&stinger_driver,	/* (c) 1983 Seibu Denshi */
	&scion_driver,		/* (c) 1984 Seibu Denshi */
	&scionc_driver,		/* (c) 1984 Seibu Denshi + Cinematronics license */
	&wiz_driver,		/* (c) 1985 Seibu Kaihatsu */

	/* Jaleco games */
	&exerion_driver,	/* (c) 1983 Jaleco */
	&exeriont_driver,	/* (c) 1983 Jaleco + Taito America license */
	&exerionb_driver,	/* bootleg */
	&formatz_driver,	/* (c) 1984 Jaleco */
	&aeroboto_driver,	/* (c) 1984 Williams */
	&citycon_driver,	/* (c) 1985 Jaleco */
	&citycona_driver,	/* (c) 1985 Jaleco */
	&cruisin_driver,		/* (c) 1985 jaleco/Kitkorp */
	&psychic5_driver,	/* (c) 1987 Jaleco */

	/* Jaleco Mega System 1 games */
	&lomakaj_driver,	/* (c) 1988 */
	&p47_driver,		/* (c) 1988 */
	&p47j_driver,		/* (c) 1988 */
	&street64_driver,	/* (c) 1991 */
	&edf_driver,		/* (c) 1991 */
	&rodlandj_driver,	/* (c) 1990 */
	&avspirit_driver,	/* (c) 1991 */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&astyanax_driver,
//	&hachoo_driver,
//	&plusalph_driver,
//	&phantasm_driver,
//	&stdragon_driver,

	/* Video System Co. games */
	&pspikes_driver,	/* (c) 1991 */
	&turbofrc_driver,	/* (c) 1991 */
	&aerofgt_driver,	/* (c) 1992 */
	&aerofgtb_driver,	/* (c) 1992 */
	&aerofgtc_driver,	/* (c) 1992 */

	/* Leland games */
/* please don't distribute binaries with these drivers uncommented. They are */
/* disabled for a reason. Don't think you are cool because you release a */
/* binary with more enabled drivers than the official MAME, any idiot with */
/* a compiler can do that. If this happens again, we'll be forced to remove */
/* unfinished drivers from the source distribution. */
//	&wseries_driver,	/* (c) 1985 Cinematronics Inc. */
//	&basebal2_driver,	/* (c) 1987 Cinematronics Inc. */
//	&dblplay_driver,	/* (c) 1987 Tradewest / The Leland Corp. */
//	&offroad_driver,	/* (c) 1989 Leland Corp. */
//	&offroadt_driver,
//	&teamqb_driver,		/* (c) 1988 Leland Corp. */
//	&strkzone_driver,	/* (c) 1988 The Leland Corporation */
//	&pigout_driver,		/* (c) 1990 The Leland Corporation */
//	&pigoutj_driver,	/* (c) 1990 The Leland Corporation */
//	&redlin2p_driver,
//	&viper_driver,
//	&aafb_driver,
//	&aafb2p_driver,
//	&ataxx_driver,

	/* Tad games */
	&cabal_driver,		/* (c) 1988 Tad + Fabtek license */
	&cabal2_driver,		/* (c) 1988 Tad + Fabtek license */
	&cabalbl_driver,	/* bootleg */
	&toki_driver,		/* (c) 1989 Tad */
	&toki2_driver,		/* (c) 1989 Tad */
	&toki3_driver,		/* (c) 1989 Tad */
	&tokiu_driver,		/* (c) 1989 Tad + Fabtek license */
	&tokib_driver,		/* bootleg */

	/* Orca games */
	&marineb_driver,	/* (c) 1982 Orca */
	&changes_driver,	/* (c) 1982 Orca */
	&springer_driver,	/* (c) 1982 Orca */
	&hoccer_driver,		/* (c) 1983 Eastern Micro Electronics, Inc. */
	&hoccer2_driver,	/* (c) 1983 Eastern Micro Electronics, Inc. */
	&funkybee_driver,	/* (c) 1982 Orca */
	&skylancr_driver,	/* (c) 1983 Orca + Esco Trading Co license */
	&zodiack_driver,	/* (c) 1983 Orca + Esco Trading Co license */
	&dogfight_driver,	/* (c) 1983 Thunderbolt */
	&moguchan_driver,	/* (c) 1982 Orca + Eastern Commerce Inc. license (doesn't appear on screen) */
	&espial_driver,		/* (c) 1983 Thunderbolt, Orca logo is hidden in title screen */
	&espiale_driver,	/* (c) 1983 Thunderbolt, Orca logo is hidden in title screen */

	&spacefb_driver,	/* (c) [1980?] Nintendo */
	&spacefbg_driver,	/* 834-0031 (c) 1980 Gremlin */
	&spacebrd_driver,	/* bootleg */
	&spacedem_driver,	/* (c) 1980 Nintendo / Fortrek */
	&blueprnt_driver,	/* (c) 1982 Bally Midway */
	&omegrace_driver,	/* (c) 1981 Midway */
	&vastar_driver,		/* (c) 1983 Sesame Japan */
	&vastar2_driver,	/* (c) 1983 Sesame Japan */
	&tankbatt_driver,	/* (c) 1980 Namco */
	&dday_driver,		/* (c) 1982 Olympia */
	&ddayc_driver,		/* (c) 1982 Olympia + Centuri license */
	&gundealr_driver,	/* (c) 1990 Dooyong */
	&gundeala_driver,	/* (c) Dooyong */
	&yamyam_driver,		/* (c) 1990 Dooyong */
	&wiseguy_driver,	/* (c) 1990 Dooyong */
	&leprechn_driver,	/* (c) 1982 Tong Electronic */
	&potogold_driver,	/* (c) 1982 Tong Electronic */
	&hexa_driver,		/* D. R. Korea */
	&redalert_driver,	/* (c) 1981 Irem (GDI game) */
	&irobot_driver,
	&spiders_driver,	/* (c) 1981 Sigma Ent. Inc. */
	&spiders2_driver,	/* (c) 1981 Sigma Ent. Inc. */
	&wanted_driver,		/* (c) 1984 Sigma Ent. Inc. */
	&stactics_driver,	/* [1981 Sega] */
	&goldstar_driver,
	&goldstbl_driver,
	&exterm_driver,		/* (c) 1989 Premier Technology - a Gottlieb game */
	&sharkatt_driver,	/* (c) Pacific Novelty */
	&kingofb_driver,	/* (c) 1985 Woodplace Inc. */
	&kingofbj_driver,	/* (c) 1985 Woodplace Inc. */
	&ringking_driver,	/* (c) 1985 Data East USA */
	&ringkin2_driver,
	&ringkin3_driver,	/* (c) 1985 Data East USA */
	&zerozone_driver,	/* (c) 1993 Comad */
	&exctsccr_driver,	/* (c) 1983 Alpha Denshi Co. */
	&exctscca_driver,	/* (c) 1983 Alpha Denshi Co. */
	&exctsccb_driver,	/* bootleg */
	&speedbal_driver,	/* (c) 1987 Tecfri */
	&sauro_driver,		/* (c) 1987 Tecfri */
	&galpanic_driver,	/* (c) 1990 Kaneko */
	&airbustr_driver,	/* (c) 1990 Kaneko */
	&ambush_driver,		/* (c) 1983 Nippon Amuse Co-Ltd */
	&starcrus_driver,	/* [1977 Ramtek] */
	&sichuan2_driver,	/* (c) 1989 Tamtex */
	&shisen_driver,		/* (c) 1989 Tamtex */
	&goindol_driver,	/* (c) 1987 Sun a Electronics */
	&homo_driver,		/* bootleg */
//	&dlair_driver,

#endif /* NEOMAME */

#ifndef NEOFREE

	/* Neo Geo games */
	&nam1975_driver,	/* (c) 1990 SNK */
	&joyjoy_driver,		/* (c) 1990 SNK */
	&mahretsu_driver,	/* (c) 1990 SNK */
	&cyberlip_driver,	/* (c) 1990 SNK */
	&tpgolf_driver,		/* (c) 1990 SNK */
	&ridhero_driver,	/* (c) 1990 SNK */
	&bstars_driver,		/* (c) 1990 SNK */
	&bstars2_driver,	/* (c) 1992 SNK */
	&ttbb_driver,		/* (c) 1991 SNK / Pallas */
	&lbowling_driver,	/* (c) 1990 SNK */
	&superspy_driver,	/* (c) 1990 SNK */
	&legendos_driver,	/* (c) 1991 SNK */
	&socbrawl_driver,	/* (c) 1991 SNK */
	&roboarmy_driver,	/* (c) 1991 SNK */
	&alpham2_driver,	/* (c) 1991 SNK */
	&eightman_driver,	/* (c) 1991 SNK / Pallas */
	&burningf_driver,	/* (c) 1991 SNK */
	&kotm_driver,		/* (c) 1991 SNK */
	&kotm2_driver,		/* (c) 1992 SNK */
	&gpilots_driver,	/* (c) 1991 SNK */
	&sengoku_driver,	/* (c) 1991 SNK */
	&sengoku2_driver,	/* (c) 1993 SNK */
	&lresort_driver,	/* (c) 1992 SNK */
	&fbfrenzy_driver,	/* (c) 1992 SNK */
	&mutnat_driver,		/* (c) 1992 SNK */
	&countb_driver,		/* (c) 1993 SNK */
	&tophuntr_driver,	/* (c) 1994 SNK */
	&aof_driver,		/* (c) 1992 SNK */
	&aof2_driver,		/* (c) 1994 SNK */
	&aof3_driver,		/* (c) 1996 SNK */
	&fatfury1_driver,	/* (c) 1991 SNK */
	&fatfury2_driver,	/* (c) 1992 SNK */
	&fatfursp_driver,	/* (c) 1993 SNK */
	&fatfury3_driver,	/* (c) 1995 SNK */
	&rbff1_driver,		/* (c) 1995 SNK */
	&rbffspec_driver,	/* (c) 1996 SNK */
	&rbff2_driver,		/* (c) 1998 SNK */
	&kof94_driver,		/* (c) 1994 SNK */
	&kof95_driver,		/* (c) 1995 SNK */
	&kof96_driver,		/* (c) 1996 SNK */
	&kof97_driver,		/* (c) 1997 SNK */
	&kof98_driver,		/* (c) 1998 SNK */
	&savagere_driver,	/* (c) 1995 SNK */
	&kizuna_driver,		/* (c) 1996 SNK */
	&samsho_driver,		/* (c) 1993 SNK */
	&samsho2_driver,	/* (c) 1994 SNK */
	&samsho3_driver,	/* (c) 1995 SNK */
	&samsho4_driver,	/* (c) 1996 SNK */
	&lastblad_driver,	/* (c) 1997 SNK */
	&lastbld2_driver,	/* (c) 1998 SNK */
	&ssideki_driver,	/* (c) 1992 SNK */
	&ssideki2_driver,	/* (c) 1994 SNK */
	&ssideki3_driver,	/* (c) 1995 SNK */
	&ssideki4_driver,	/* (c) 1996 SNK */
	&mslug2_driver,		/* (c) 1998 SNK */
	&bjourney_driver,	/* (c) 1990 Alpha Denshi Co */
	&maglord_driver,	/* (c) 1990 Alpha Denshi Co */
	&maglordh_driver,	/* (c) 1990 Alpha Denshi Co */
	&ncombat_driver,	/* (c) 1990 Alpha Denshi Co */
	&crsword_driver,	/* (c) 1991 Alpha Denshi Co */
	&trally_driver,		/* (c) 1991 Alpha Denshi Co */
	&ncommand_driver,	/* (c) 1992 Alpha Denshi Co */
	&wh1_driver,		/* (c) 1992 Alpha Denshi Co */
	&wh2_driver,		/* (c) 1993 ADK */
	&wh2j_driver,		/* (c) 1994 ADK / SNK */
	&whp_driver,		/* (c) 1995 ADK / SNK */
	&aodk_driver,		/* (c) 1994 ADK / SNK */
	&ninjamas_driver,	/* (c) 1996 ADK / SNK */
	&overtop_driver,	/* (c) 1996 ADK */
	&twinspri_driver,	/* (c) 1996 ADK */
	&janshin_driver,	/* (c) 1994 Aicom */
	&pulstar_driver,	/* (c) 1995 Aicom */
	&spinmast_driver,	/* (c) 1993 Data East Corporation */
	&karnovr_driver,	/* (c) 1994 Data East Corporation */
	&wjammers_driver,	/* (c) 1994 Data East Corporation */
	&strhoops_driver,	/* (c) 1994 Data East Corporation */
	&magdrop2_driver,	/* (c) 1996 Data East Corporation */
	&magdrop3_driver,	/* (c) 1997 Data East Corporation */
	&gururin_driver,	/* (c) 1994 Face */
	&miexchng_driver,	/* (c) 1997 Face */
	&panicbom_driver,	/* (c) 1994 Eighting / Hudson */
	&kabukikl_driver,	/* (c) 1995 Hudson */
	&neobombe_driver,	/* (c) 1997 Hudson */
	&minasan_driver,	/* (c) 1990 Monolith Corp. */
	&bakatono_driver,	/* (c) 1991 Monolith Corp. */
	&mslug_driver,		/* (c) 1996 Nazca */
	&turfmast_driver,	/* (c) 1996 Nazca */
	&zedblade_driver,	/* (c) 1994 NMK */
	&viewpoin_driver,	/* (c) 1992 Sammy */
	&quizkof_driver,	/* (c) 1995 Saurus */
	&stakwin_driver,	/* (c) 1995 Saurus */
	&stakwin2_driver,	/* (c) 1996 Saurus */
	&ragnagrd_driver,	/* (c) 1996 Saurus */
	&shocktro_driver,	/* (c) 1997 Saurus */
	&galaxyfg_driver,	/* (c) 1995 Sunsoft */
	&wakuwak7_driver,	/* (c) 1996 Sunsoft */
	&pbobble_driver,	/* (c) 1994 Taito */
	&marukodq_driver,	/* (c) 1995 Takara */
	&doubledr_driver,	/* (c) 1995 Technos */
	&gowcaizr_driver,	/* (c) 1995 Technos */
	&tws96_driver,		/* (c) 1996 Tecmo */
	&blazstar_driver,	/* (c) 1998 Yumekobo */
	&pspikes2_driver,	/* (c) 1994 Video System Co. */
	&sonicwi2_driver,	/* (c) 1994 Video System Co. */
	&sonicwi3_driver,	/* (c) 1995 Video System Co. */
	&androdun_driver,	/* (c) 1992 Visco */
	&goalx3_driver,		/* (c) 1995 Visco */
	&puzzledp_driver,	/* (c) 1995 Taito (Visco license) */
	&puzzldpr_driver,	/* (c) 1997 Taito (Visco license) */
	&neodrift_driver,	/* (c) 1996 Visco */
	&neomrdo_driver,	/* (c) 1996 Visco */
	&breakers_driver,	/* (c) 1996 Visco */
	&breakrev_driver,	/* (c) 1998 Visco */
	&flipshot_driver,	/* (c) 1998 Visco */

#endif /* NEOFREE */

	0	/* end of array */
};

#endif /* TINY_COMPILE */
