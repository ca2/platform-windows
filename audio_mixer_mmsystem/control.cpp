#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      control::control()
      {

         m_pmixersource = nullptr;
         m_mixercontrol.cbStruct = sizeof(MIXERCONTROL);
         m_mixercontroldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
         m_mixercontroldetails.paDetails = nullptr;
      }


      control::~control()
      {
         if(m_mixercontroldetails.paDetails != nullptr)
            free(m_mixercontroldetails.paDetails);

         ::multimedia::audio_mixer::user::control * pinteraction;

         for(i32 i = 0; i < this->get_size(); i++)
         {

            if((pinteraction = this->element_at(i)) != nullptr)
            {

               delete pinteraction;

            }

         }

      }

      bool control::CreateWindows(__pointer(::user::interaction) pParent, i32 iStyle)
      {
         UNREFERENCED_PARAMETER(pParent);
         UNREFERENCED_PARAMETER(iStyle);
         return true;
      }



      bool control::CreateWindowsVolumeV001(
      __pointer(::user::interaction)pParent,
      u32 nStartID,
      u32 * nNextID)
      {
         ASSERT(m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME);

         static wchar_t        szScrollBar[] = L"scrollbar";
         static wchar_t        szTitle[]     = L"Fader Class: '%s'";

         u32                        cb;
         u32                        lcChannels;
         u32                        lcMultipleItems;
         i32                         nRange;
         i32                         nPageInc;

         __pointer(::multimedia::audio_mixer_mmsystem::source) source = m_pmixersource;

         ASSERT(source != nullptr);

         lcChannels = (u32)source->m_mixerline.cChannels;
         if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
            m_mixercontroldetails.cChannels = 1;

         lcMultipleItems = 1;
         if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
            lcMultipleItems = (u32)m_mixercontrol.cMultipleItems;

         if(lcChannels > 2)
            return false;
         if(lcMultipleItems != 1)
            return false;

         cb = lcChannels * lcMultipleItems * sizeof(MIXERCONTROLDETAILS_UNSIGNED);
         m_mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
         if(m_mixercontroldetails.paDetails != nullptr)
            free(m_mixercontroldetails.paDetails);
         m_mixercontroldetails.paDetails = malloc(cb);

         nRange = (i32) minimum(32767, m_mixercontrol.Metrics.cSteps - 1);
         bool bOk = false;
         for(i32 i = 5; i <= 10; i++)
         {
            nPageInc = nRange / i;
            if(nPageInc < 1)
            {
               bOk = true;
               nPageInc = 1;
               break;
            }
            if(nRange % i == 0)
            {
               bOk = true;
               break;
            }
         }
         if(!bOk)
         {
            for(i32 i = 5; i <= 10; i++)
            {
               nPageInc = nRange / i;
               if(nRange % i > (nPageInc / 2))
               {
                  bOk = true;
                  break;
               }
            }
         }

         u32 nID = nStartID;
         u32 nVolumeID = nID++;
         u32 nVolumeLabelID = nID++;
         u32 nBalanceID = nID++;
         u32 nBalanceLabelID = nID++;

         ::rectangle_i32 rectangle(0, 0, 0, 0);

         __pointer(::multimedia::audio_mixer::user::level_control) plevelcontrol = source->get_device()->get_mixer()->get_audio_mixer_callback()->allocate_level_control();
         if (plevelcontrol == nullptr)
         {
            return false;

         }
         plevelcontrol->create_control(pParent, nVolumeID);
         __pointer(::multimedia::audio_mixer::control_data_volume) mcdvVolume = plevelcontrol->get_data();
         mcdvVolume->m_iType = ::multimedia::audio_mixer::control_data::TypeStereoVolume;
         mcdvVolume->m_uiMixerID = source->get_device()->m_uiMixerID;
         mcdvVolume->m_uiLineID = source->m_mixerline.dwLineID;
         mcdvVolume->m_uiControlID = m_mixercontrol.dwControlID;
         mcdvVolume->nRange  = nRange;
         mcdvVolume->nPageInc = nPageInc;
         plevelcontrol->SetOrientation(::multimedia::audio_mixer::user::level_control::e_orientation_vertical);
         plevelcontrol->SetRange(0,mcdvVolume->nRange);
         plevelcontrol->SetLineSize(1);
         plevelcontrol->SetPageSize(mcdvVolume->nPageInc);
         plevelcontrol->__compose(plevelcontrol->m_pdata, mcdvVolume);
         add(plevelcontrol.m_p);


         string str;
         __pointer(::multimedia::audio_mixer::user::label) plabelVolume = source->get_device()->get_mixer()->get_audio_mixer_callback()->allocate_label();
         if(plabelVolume == nullptr)
            return false;
         plabelVolume->create_control(pParent, nVolumeLabelID);
         //xxx   str.load_string(IDS_MIXER_VOLUME);
         plabelVolume->set_window_text(str);
         __pointer(::multimedia::audio_mixer::control_data) mcdVolumeLabel = plabelVolume->get_data();
         mcdVolumeLabel->m_iType = ::multimedia::audio_mixer::control_data::TypeStereoVolumeLabel;
         mcdVolumeLabel->m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
         mcdVolumeLabel->m_uiLineID = source->m_mixerline.dwLineID;
         mcdVolumeLabel->m_uiControlID = m_mixercontrol.dwControlID;
         add(plabelVolume.m_p);


         if(lcChannels == 2)
         {
            __pointer(::multimedia::audio_mixer::user::level_control) pinteraction = source->get_device()->get_mixer()->get_audio_mixer_callback()->allocate_level_control();
            if(pinteraction == nullptr)
               return false;

            pinteraction->create_control(pParent, nBalanceID);
            __pointer(::multimedia::audio_mixer::control_data_volume) mcdvBalance = pinteraction->get_data();
            mcdvBalance->m_iType = ::multimedia::audio_mixer::control_data::TypeStereoBalance;
            mcdvBalance->m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
            mcdvBalance->m_uiLineID = source->m_mixerline.dwLineID;
            mcdvBalance->m_uiControlID = m_mixercontrol.dwControlID;
            mcdvBalance->nRange  = nRange;
            mcdvBalance->nPageInc = nPageInc;

            pinteraction->SetOrientation(::multimedia::audio_mixer::user::level_control::e_orientation_horizontal);
            pinteraction->SetRange(0,mcdvBalance->nRange);
            pinteraction->SetLineSize(1);
            pinteraction->SetPageSize(mcdvBalance->nPageInc);

            add(pinteraction.m_p);

            __pointer(::multimedia::audio_mixer::user::label) plabel = m_pmixersource->get_device()->get_mixer()->get_audio_mixer_callback()->allocate_label();
            if(plabel == nullptr)
               return false;

            plabel->create_control(pParent, nBalanceLabelID);
            __pointer(::multimedia::audio_mixer::control_data) mcdBalanceLabel = plabel->get_data();
            mcdBalanceLabel->m_iType = ::multimedia::audio_mixer::control_data::TypeStereoBalanceLabel;
            mcdBalanceLabel->m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
            mcdBalanceLabel->m_uiLineID = source->m_mixerline.dwLineID;
            mcdBalanceLabel->m_uiControlID = m_mixercontrol.dwControlID;
            //xxx      str.load_string(IDS_MIXER_BALANCE);
            plabel->set_window_text(str);
            add(plabel.m_p);
         }

         *nNextID = nID;
         return true;
      }

      bool control::_001CreateMuteControl(
      __pointer(::user::interaction)pParent,
      u32 nStartID,
      u32 * nNextID)
      {
         ASSERT((m_mixercontrol.dwControlType &  MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN);

         u32                        cb;
         u32                        lcChannels;
         u32                        lcMultipleItems;


         ASSERT(m_pmixersource != nullptr);

         __pointer(::multimedia::audio_mixer_mmsystem::source) source = m_pmixersource;

         lcChannels = (u32)source->m_mixerline.cChannels;
         if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
            lcChannels = 1;

         lcMultipleItems = 1;
         if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
            lcMultipleItems = (u32)m_mixercontrol.cMultipleItems;

         //if(lcChannels > 2)
         //  return false;
         //if(lcMultipleItems != 1)
         //  return false;

         i32 iItemCount = lcChannels * lcMultipleItems;

         cb = iItemCount * sizeof(MIXERCONTROLDETAILS_BOOLEAN);
         m_mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
         if(m_mixercontroldetails.paDetails != nullptr)
            free(m_mixercontroldetails.paDetails);
         m_mixercontroldetails.paDetails = malloc(cb);

         u32 nID = nStartID;
         u32 nMuteID;

         ::text::text text;

         auto pcontext = pParent->m_pcontext;
         
         if(m_pmixersource == m_pmixersource->get_destination())
         {
            
            text = pcontext->__text("mix::mute_all");

         }
         else
         {
            
            text = pcontext->__text("mix::mute");

         }

         for(i32 iItem = 0; iItem < iItemCount; iItem++)
         {
            nMuteID = nID++;
            __pointer(::multimedia::audio_mixer::user::toggle_control) pbtMute = m_pmixersource->get_device()->get_mixer()->get_audio_mixer_callback()->allocate_toggle_control();
            pbtMute->create_control(pParent, nMuteID);
            pbtMute->set_window_text(text.get_text());
            __pointer(::multimedia::audio_mixer::control_data_switch) mcdmMute = pbtMute->get_data();
            mcdmMute->m_iType = ::multimedia::audio_mixer::control_data::TypeUniformMute;
            mcdmMute->m_uiMixerID = source->get_device()->m_uiMixerID;
            mcdmMute->m_uiControlID = m_mixercontrol.dwControlID;
            mcdmMute->m_uiLineID = ((MIXERLINE *)m_pmixersource)->dwLineID;
            add(pbtMute.m_p);
         }

         *nNextID = nID;
         return true;
      }

      /*void control::OnArrayReallocation(void *pNewPointer)
      {
      m_pmixersource = (::multimedia::audio_mixer::source *) pNewPointer;
      }
      */

      void control::Prepare()
      {
         m_mixercontroldetails.dwControlID = m_mixercontrol.dwControlID;
      }

      /*      __pointer(::multimedia::audio_mixer::user::control) control::GetControl(i32 iType)
            {
               ::multimedia::audio_mixer::user::control  * pusercontrol = nullptr;
               for(i32 i = 0; i < this->get_size(); i++)
               {
                  pusercontrol = this->element_at(i);
                  if(pusercontrol == nullptr)
                     continue;
                  if(pusercontrol->m_pdata->get_type() == iType)
                  {
                     return pusercontrol;
                  }
               }
               return nullptr;
            }

            ::multimedia::audio_mixer::user::control * control::GetControlByIndex(::index iIndex)
            {
               return this->element_at(iIndex);
            }
            */

      void control::OnMixerControlChange()
      {
         if(this->get_size() <= 0)
            return;
         ::e_status                            mmrc;
         //    oswindow                            htxt;
         //    PMACONTROLINSTANCE_FADER        pmaci_fader;
         //    LPMACONTROLINSTANCE             pmaci;
         //    LPMIXERLINE                     pmxl;
         //    LPMIXERCONTROL                  pmxctrl;
         PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u;
         i32                             nRange;
         //    i32                             nValue;
         u32                            cChannels;
         u32                            cMultipleItems;
         //    u32                            u;
         //    u32                            uIndex;
         //    u32                            v;
         //    MIXERCONTROLDETAILS             mxcd;
         //   oswindow                            hsbFocus;
         //    oswindow                            hsb;


         //    hsbFocus = GetFocus();
         //    if (nullptr == hsbFocus)
         //    {
         //        hsbFocus = get_child_by_id(oswindow, IDD_MACONTROL_MULTICHANNEL_BASE);
         //    }
         //    else
         //    {
         //        uIndex = GetDlgCtrlId(hsbFocus);
         //        if (uIndex < IDD_MACONTROL_MULTICHANNEL_BASE)
         //        {
         //            hsbFocus = get_child_by_id(oswindow, IDD_MACONTROL_MULTICHANNEL_BASE);
         //        }
         //    }


         //
         //
         //
         //    pmaci_fader = (PMACONTROLINSTANCE_FADER)(u32)GetWindowLong(oswindow, DWL_USER);
         //    pmaci       = pmaci_fader->pmaci;
         //    pmxl        = pmaci->pmxl;
         //    pmxctrl     = pmaci->pmxctrl;
         //    pmxcd_u     = &pmaci_fader->pmxcd_u[0];
         //    nRange      = pmaci_fader->nRange;

         __pointer(::multimedia::audio_mixer_mmsystem::source) source = m_pmixersource;
         __pointer(::multimedia::audio_mixer_mmsystem::device) device = source->get_device();

         cChannels = (u32)source->m_mixerline.cChannels;
         if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
            cChannels = 1;

         m_mixercontroldetails.cChannels = cChannels;


         //
         //
         //
         //    m_mixercontroldetails.cChannels      = cChannels;
         m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;



         mmrc = mmsystem::translate(mixerGetControlDetails((HMIXEROBJ) device->m_hMixer, &m_mixercontroldetails, MIXER_GETCONTROLDETAILSF_VALUE));

         if (::success != mmrc)
         {
            //        message_box(nullptr, MB_OK | e_message_box_icon_exclamation,
            //                "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
            //              m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
            return;
         }

         cMultipleItems = 1;

         if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
            cMultipleItems = (u32)m_mixercontrol.cMultipleItems;

         if(m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME)
         {
            ASSERT(cMultipleItems == 1);
            if(cChannels == 2)
            {
               __pointer(::multimedia::audio_mixer::user::level_control) pslVolume = GetControl(::multimedia::audio_mixer::control_data::TypeStereoVolume);
               __pointer(::multimedia::audio_mixer::control_data_volume) lpmcdVolume = GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);
               __pointer(::multimedia::audio_mixer::user::level_control) pslBalance = GetControl(::multimedia::audio_mixer::control_data::TypeStereoBalance);
               double dActualBalance = pslBalance->GetPos();

               nRange = lpmcdVolume->nRange;
               pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
               i32 nLeftValue = (i32)MulDiv(pmxcd_u[0].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);
               i32 nRightValue = (i32)MulDiv(pmxcd_u[1].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);


               i32 nMaxValue = maximum(nLeftValue, nRightValue);
               i32 nBalance = nMaxValue == 0 ?
                                  nRange / 2 :
                                  (nRange +
                                   MulDiv(nRightValue - nLeftValue, nRange, nMaxValue)) / 2;


               pslVolume->SetPos(nRange - nMaxValue);

               double dRangeSlack = nMaxValue == 0 ?
                                    nRange / 2 :
                                    ((double) nRange) / (2 * nMaxValue) + 0.5;
               if(dActualBalance > nBalance + dRangeSlack ||
                     dActualBalance < nBalance - dRangeSlack)
               {
                  pslBalance->SetPos(nBalance);
               }
               TRACE("Left %d Right %d nBalance %d\n", nLeftValue, nRightValue, nBalance);
            }
            else if(cChannels == 1)
            {
               __pointer(::multimedia::audio_mixer::user::level_control) pslVolume = GetControl(::multimedia::audio_mixer::control_data::TypeStereoVolume);
               __pointer(::multimedia::audio_mixer::control_data_volume) lpmcdVolume =  GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);

               nRange = lpmcdVolume->nRange;
               pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
               i32 nValue = (i32)MulDiv(pmxcd_u[0].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);

               pslVolume->SetPos(nRange - nValue);
            }
         }
         else if((m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN)
         {
            for(i32 i = 0; i < this->get_size(); i++)
            {
               __pointer(::multimedia::audio_mixer::user::toggle_control) pmutecontrol =  GetControlByIndex(i);


               PMIXERCONTROLDETAILS_BOOLEAN pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) m_mixercontroldetails.paDetails;
               bool fValue = pmxcd_f[cMultipleItems - i - 1].fValue != false;

               pmutecontrol->_001SetCheck(fValue ? check_checked : check_unchecked, ::e_source_user);
            }
         }
      }

      /*
      ::multimedia::audio_mixer::control_data * control::GetWindowData(i32 iType)
      {
         ::multimedia::audio_mixer::user::control * pusercontrol = nullptr;
         for(i32 i = 0; i < this->get_size(); i++)
         {
            pusercontrol = this->element_at(i);
            if(pusercontrol == nullptr)
               continue;
            if(pusercontrol->m_pdata->get_type() == iType)
            {
               return pusercontrol->m_pdata;
            }
         }
         return nullptr;
      }*/

      /*bool control::OnNotify(u32 nID, LPNMHDR lpnmhdr)
      {
      UNREFERENCED_PARAMETER(nID);
      if(lpnmhdr->code == TB_BOTTOM ||
      lpnmhdr->code == TB_ENDTRACK ||
      lpnmhdr->code == TB_LINEDOWN ||
      lpnmhdr->code == TB_LINEUP ||
      lpnmhdr->code == TB_PAGEDOWN ||
      lpnmhdr->code == TB_PAGEUP ||
      lpnmhdr->code == TB_THUMBPOSITION ||
      lpnmhdr->code == TB_THUMBTRACK ||
      lpnmhdr->code == TB_TOP)
      {
      audio_mixer::control * pinteraction = GetControlByDlgCtrlID(lpnmhdr->idFrom);
      if(pinteraction != nullptr)
      {
      //            ::multimedia::audio_mixer::user::level_control * plevelcontrol = (::multimedia::audio_mixer::user::level_control *) pinteraction;
      /*if(base_class <CSliderCtrl >::bases(pusercontrol))
      {
      CSliderCtrl * pSlider = (CSliderCtrl *) pusercontrol;
      ::multimedia::audio_mixer::control_data * pData;
      if(nullptr != (pData = GetWindowDataByDlgCtrlID(lpnmhdr->idFrom)))
      {

      if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoBalance ||
      pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoVolume)
      {
      CSliderCtrl * pslBalance;
      CSliderCtrl * pslVolume;
      if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoBalance)
      {
      pslBalance = pSlider;
      pslVolume = (CSliderCtrl *) GetControl(::multimedia::audio_mixer::control_data::TypeStereoVolume)->GetWnd();
      ASSERT(pslVolume != nullptr);
      }
      else if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoVolume)
      {
      pslVolume = pSlider;
      pslBalance = (CSliderCtrl *) GetControl(::multimedia::audio_mixer::control_data::TypeStereoBalance)->GetWnd();
      ASSERT(pslVolume != nullptr);
      }
      m_mixercontroldetails.cChannels = (u32)m_pmixersource->m_mixerline.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
      m_mixercontroldetails.cChannels = 1;

      //
      //
      //
      //                        m_mixercontroldetails.cChannels      = cChannels;
      m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
      if(m_mixercontroldetails.cChannels == 2)
      {
      ::multimedia::audio_mixer::control_data_volume * lpmcdVolume = (::multimedia::audio_mixer::control_data_volume *) GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);
      i32 nRange = lpmcdVolume->nRange;
      i32 nBalance  = pslBalance->GetPos();
      i32 nMaxValue = nRange - pslVolume->GetPos();
      i32 nMinValue = nBalance * 2 * nMaxValue / nRange;
      i32 nLeftValue;
      i32 nRightValue;
      if(nBalance == nRange / 2)
      {
      nLeftValue = nMaxValue;
      nRightValue = nMaxValue;
      }
      else if(nBalance > nRange / 2)
      {
      nLeftValue = nMinValue;
      nRightValue = nMaxValue;
      }
      else
      {
      nLeftValue = nMaxValue;
      nRightValue = nMinValue;
      }

      PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
      pmxcd_u[0].dwValue = (i32)MulDiv(
      nLeftValue ,
      m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
      nRange) +  m_mixercontrol.Bounds.dwMinimum;
      pmxcd_u[1].dwValue = (i32)MulDiv(
      nRightValue ,
      m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
      nRange) +  m_mixercontrol.Bounds.dwMinimum;
      ::e_status     mmrc = mixerSetControlDetails(
      (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
      &m_mixercontroldetails,
      MIXER_GETCONTROLDETAILSF_VALUE);
      if (MMSYSERR_NOERROR == mmrc)
      {
      OnMixerControlChange();
      }
      else
      {
      //                                message_box(nullptr, MB_OK | e_message_box_icon_exclamation,
      //                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
      //                                      m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
      }

      }
      else if(m_mixercontroldetails.cChannels == 1)
      {
      CSliderCtrl * pslVolume = (CSliderCtrl *) GetControl(::multimedia::audio_mixer::control_data::TypeStereoVolume)->GetWnd();
      ::multimedia::audio_mixer::control_data_volume * lpmcdVolume = (::multimedia::audio_mixer::control_data_volume *) GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);
      i32 nRange = lpmcdVolume->nRange;
      i32 nValue = nRange - pslVolume->GetPos();
      PMIXERCONTROLDETAILS_UNSIGNED pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
      pmxcd_u[0].dwValue = (i32)MulDiv(nValue, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum, nRange) + m_mixercontrol.Bounds.dwMinimum;
      ::e_status     mmrc = mixerSetControlDetails(
      (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
      &m_mixercontroldetails,
      MIXER_GETCONTROLDETAILSF_VALUE);
      if (MMSYSERR_NOERROR == mmrc)
      {
      OnMixerControlChange();
      }
      else
      {
      //                                message_box(nullptr, MB_OK | e_message_box_icon_exclamation,
      ///                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
      //                                     m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
      }
      }
      return true;
      }
      }*/

      //            }
      /*}

      }
      return false;

      }*/

      /*

            ::multimedia::audio_mixer::user::control * control::GetControlByDlgCtrlID(u32 nID)
            {
               ::multimedia::audio_mixer::user::control * pusercontrol = nullptr;
               for(i32 i = 0; i < this->get_size(); i++)
               {
                  pusercontrol = this->element_at(i);
                  if(pusercontrol == nullptr)
                     continue;
                  if((u32) pusercontrol->GetDlgCtrlId() == nID)
                  {
                     return pusercontrol;
                  }
               }
               return nullptr;

            }

            ::multimedia::audio_mixer::control_data * control::GetWindowDataByDlgCtrlID(u32 nID)
            {
               ::multimedia::audio_mixer::user::control * pusercontrol = nullptr;
               for(i32 i = 0; i < this->get_size(); i++)
               {
                  pusercontrol = this->element_at(i);
                  if(pusercontrol == nullptr)
                     continue;
                  if((u32) pusercontrol->GetDlgCtrlId() == nID)
                  {
                     return pusercontrol->m_pdata;
                  }
               }
               return nullptr;
            }
            */

      void control::OnVHScroll(u32 nSBCode, u32 nPos, __pointer(::user::interaction) pScrollBar)
      {
         UNREFERENCED_PARAMETER(nPos);
         __pointer(::user::interaction) pParamWnd = pScrollBar;
         if(nSBCode == SB_LEFT ||
               nSBCode == SB_ENDSCROLL ||
               nSBCode == SB_LINELEFT ||
               nSBCode == SB_LINERIGHT ||
               nSBCode == SB_PAGELEFT ||
               nSBCode == SB_PAGERIGHT ||
               nSBCode == SB_THUMBPOSITION ||
               nSBCode == SB_THUMBTRACK ||
               nSBCode == SB_RIGHT ||
               nSBCode == SB_BOTTOM ||
               nSBCode == SB_ENDSCROLL ||
               nSBCode == SB_LINEUP ||
               nSBCode == SB_LINEDOWN ||
               nSBCode == SB_PAGEUP ||
               nSBCode == SB_PAGEDOWN ||
               nSBCode == SB_TOP )
         {
            ::multimedia::audio_mixer::user::control * pinteraction = GetControlByDlgCtrlID((u32) pParamWnd->GetDlgCtrlId());
            if(pinteraction != nullptr)
            {
               ::multimedia::audio_mixer::user::level_control * pSlider = dynamic_cast<::multimedia::audio_mixer::user::level_control*>(pinteraction);
               if(nullptr != pSlider)
               {
                  //mix::SliderInterface * pSlider = dynamic_cast < ::multimedia::audio_mixer::user::level_control * > ( pusercontrol);
                  ::multimedia::audio_mixer::control_data * pData;
                  if(nullptr != (pData = GetWindowDataByDlgCtrlID((u32) pParamWnd->GetDlgCtrlId())))
                  {

                     if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoBalance ||
                           pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoVolume)
                     {
                        __pointer(::multimedia::audio_mixer_mmsystem::source) source = m_pmixersource;
                        __pointer(::multimedia::audio_mixer_mmsystem::device) device = source->get_device();
                        __pointer(::multimedia::audio_mixer::user::level_control) pslBalance;
                        __pointer(::multimedia::audio_mixer::user::level_control) pslVolume;
                        if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoBalance)
                        {
                           pslBalance = pSlider;
                           pslVolume = GetControl(::multimedia::audio_mixer::control_data::TypeStereoVolume);
                           ASSERT(pslVolume != nullptr);
                        }
                        else if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeStereoVolume)
                        {
                           pslVolume = pSlider;
                           pslBalance = GetControl(::multimedia::audio_mixer::control_data::TypeStereoBalance);
                           ASSERT(pslVolume != nullptr);
                        }
                        m_mixercontroldetails.cChannels = (u32)source->m_mixerline.cChannels;
                        if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
                           m_mixercontroldetails.cChannels = 1;

                        //
                        //
                        //
                        //m_mixercontroldetails.cChannels      = cChannels;
                        m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
                        if(m_mixercontroldetails.cChannels == 2)
                        {
                           __pointer(::multimedia::audio_mixer::control_data_volume) lpmcdVolume = GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);
                           i32 nRange = lpmcdVolume->nRange;
                           i32 nBalance  = pslBalance->GetPos();
                           i32 nMaxValue = nRange - pslVolume->GetPos();
                           i32 nLeftValue;
                           i32 nRightValue;
                           if(nBalance == nRange / 2)
                           {
                              nLeftValue = nMaxValue;
                              nRightValue = nMaxValue;
                           }
                           else if(nBalance > nRange / 2)
                           {
                              nLeftValue =  2 * nMaxValue - MulDiv(nBalance, 2 * nMaxValue, nRange);
                              nRightValue = nMaxValue;
                           }
                           else
                           {
                              nLeftValue = nMaxValue;
                              nRightValue = MulDiv(nBalance, 2 * nMaxValue, nRange);
                           }

                           PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
                           pmxcd_u[0].dwValue = (i32)MulDiv(
                                                nLeftValue,
                                                m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
                                                nRange) +  m_mixercontrol.Bounds.dwMinimum;
                           pmxcd_u[1].dwValue = (i32)MulDiv(
                                                nRightValue,
                                                m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
                                                nRange) +  m_mixercontrol.Bounds.dwMinimum;

                           ::e_status     mmrc = mmsystem::translate(mixerSetControlDetails((HMIXEROBJ) device->m_hMixer, &m_mixercontroldetails, MIXER_GETCONTROLDETAILSF_VALUE));

                           if (::success == mmrc)
                           {

                              OnMixerControlChange();

                           }
                           else
                           {
                              //                                message_box(nullptr, MB_OK | e_message_box_icon_exclamation,
                              //                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                              //                                      m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
                           }

                        }
                        else if(m_mixercontroldetails.cChannels == 1)
                        {
                           //CSliderCtrl * pslVolume = (CSliderCtrl *) GetWindow(::multimedia::audio_mixer::control_data::TypeStereoVolume);
                           __pointer(::multimedia::audio_mixer::control_data_volume) lpmcdVolume = GetWindowData(::multimedia::audio_mixer::control_data::TypeStereoVolume);
                           i32 nRange = lpmcdVolume->nRange;
                           i32 nValue = nRange - pslVolume->GetPos();
                           PMIXERCONTROLDETAILS_UNSIGNED pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
                           pmxcd_u[0].dwValue = (i32)MulDiv(nValue, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum, nRange) + m_mixercontrol.Bounds.dwMinimum;

                           ::e_status     mmrc = mmsystem::translate(mixerSetControlDetails((HMIXEROBJ) device->m_hMixer, &m_mixercontroldetails, MIXER_GETCONTROLDETAILSF_VALUE));

                           if(::success == mmrc)
                           {
                              OnMixerControlChange();
                           }
                           else
                           {
                              string strMessage;
                              strMessage.Format("mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                                                m_mixercontrol.dwControlID, device->m_hMixer, mmrc);
                              pScrollBar->message_box(strMessage, nullptr, e_message_box_icon_exclamation);
                           }
                        }
                        return ;
                     }
                  }

               }
            }

         }
      }

      bool control::OnCommand(WPARAM wParam, LPARAM lParam)
      {
         UNREFERENCED_PARAMETER(lParam);
         WORD wNotifyCode = HIWORD(wParam);
         WORD wID = LOWORD(wParam);

         __pointer(::multimedia::audio_mixer::user::control) puserinteraction = GetControlByDlgCtrlID((u32)wID);

         __pointer(::multimedia::audio_mixer::user::toggle_control) pmutecontrol =puserinteraction;

         if(pmutecontrol != nullptr)
         {
            __pointer(::multimedia::audio_mixer::control_data) pData;
            __throw(todo, "find replacement/fix for GetWindowDataByDlgCtrlID");
            //if(nullptr != (pData = GetWindowDataByDlgCtrlID((u32) wID)))
            {
               if(pData->get_type() == ::multimedia::audio_mixer::control_data::TypeUniformMute)
               {
                  if(wNotifyCode == BN_CLICKED)
                  {

                     __pointer(::multimedia::audio_mixer_mmsystem::source) source = m_pmixersource;
                     __pointer(::multimedia::audio_mixer_mmsystem::device) device = source->get_device();

                     m_mixercontroldetails.cChannels = (u32)source->m_mixerline.cChannels;
                     if(MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
                        m_mixercontroldetails.cChannels = 1;


                     ::index iSel = pmutecontrol->get_data()->m_iIndex;

                     //m_mixercontroldetails.cChannels      = cChannels;
                     m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
                     i32 cMultipleItems = m_mixercontrol.cMultipleItems;
                     PMIXERCONTROLDETAILS_BOOLEAN   pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) m_mixercontroldetails.paDetails;
                     for(i32 i = 0; i < this->get_size(); i++)
                     {
                        bool fValue = i == iSel ? 1 : 0;
                        pmxcd_f[cMultipleItems - i - 1].fValue = fValue;
                     }

                     ::e_status     mmrc = mmsystem::translate(mixerSetControlDetails((HMIXEROBJ) device->m_hMixer, &m_mixercontroldetails, MIXER_GETCONTROLDETAILSF_VALUE));

                     if(::success == mmrc)
                     {

                        OnMixerControlChange();

                     }
                     else
                     {

                        string strMessage;

                        strMessage.Format("mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                                          m_mixercontrol.dwControlID, device->m_hMixer, mmrc);
                        pmutecontrol->message_box(strMessage, nullptr, e_message_box_icon_exclamation);
                     }
                     return true;
                  }
               }
            }
         }
         return false;
      }



      bool control::control_type(::multimedia::audio_mixer::e_control econtrol) const
      {

         switch(econtrol)
         {
         case ::multimedia::audio_mixer::control_boolean:
            return (m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN;
         case ::multimedia::audio_mixer::control_volume:
            return m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME;
         case ::multimedia::audio_mixer::control_mux:
            return m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_MUX;
         default:
            return ::multimedia::audio_mixer::control::control_type(econtrol);
         };

      }


      iptr control::add(::multimedia::audio_mixer::user::control * pinteraction)
      {
         pinteraction->get_data()->m_iIndex = this->get_size();
         control_ptr_array::add(pinteraction);
         return control_ptr_array::get_upper_bound();
      }


      MIXERCONTROL & control::GetMixerControl()
      {
         return m_mixercontrol;
      }

      MIXERCONTROLDETAILS & control::GetMixerControlDetails()
      {
         return m_mixercontroldetails;
      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




