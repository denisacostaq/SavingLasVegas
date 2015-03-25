/*! \brief This file have the implementation for AtomicBomb class
    \file atomicbomb.cc
    \author Copyright owners
    \date 6/21/2014

    \copyright

    \attention <h1><center><strong>COPYRIGHT &copy; 2015 </strong>
    [<strong>Alvaro Denis</strong>][DENISACOSTAQ-URL] &
    [<strong>Luis Teijon</strong>][GDSRAM-URL]. All rights reserved.</center></h1>
    \attention This file is part of [<strong>Saving Las Vegas Project</strong>][SAVINGLASVEGAS-URL].

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    [DENISACOSTAQ-URL]: https://about.me/denisacostaq/ "Software Developer."
    [GDSRAM-URL]: https://about.me/gdsram/ "Software Developer."
    [SAVINGLASVEGAS-URL]: https://savinglasvegas.com/ "A cocos2d-x based game."
 */

#include "Classes/GameObject/atomicbomb.h"

AtomicBomb::AtomicBomb(
    const std::function<void(CC::Node*)> &weaponExplodedCallback,
    const std::function<void()> &weaponDischargedCallback)
  : GenericWeapon{weaponExplodedCallback}
  , weapon_discharged_callback_{weaponDischargedCallback} {
}

AtomicBomb* AtomicBomb::create(
    const std::function<void(CC::Node*)> &weaponExplodedCallback,
    const std::function<void()> &weaponDischargedCallback) noexcept {
  AtomicBomb *pRet{
    new AtomicBomb{weaponExplodedCallback, weaponDischargedCallback}};
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

void AtomicBomb::putIn(
  const CC::Vec2  &position,
  const std::function<void(GenericWeapon*)> &explodeWeapon) noexcept {
  GenericWeapon::putIn(position);
  this->runAction(
        CC::Sequence::create(
          CC::ScaleTo::create(3.5f, 1.f),
          CC::DelayTime::create(3.f),
          CC::CallFunc::create([this, &explodeWeapon](){
    explodeWeapon(this);
  }),
          nullptr));
  CC::Sequence *seq{
    CC::Sequence::createWithTwoActions(
          CC::DelayTime::create(1),
          CC::CallFunc::create(
            CC_CALLBACK_0(GenericWeapon::activeBomb, this)))};
  this->runAction(seq);
}

void AtomicBomb::explode(CC::CallFuncN* onExploded) noexcept {
  // no es necasario ya que en initAtTheB de generic
  neutralized_menaces_by_last_shoot_ = 0;
  this->stopAllActions();
  float scaleFactor{getBoundingBox().size.width / getContentSize().width};
  this->setScale(.3f);
  CC::Sequence *seq{
    CC::Sequence::create(
          CC::ScaleTo::create(scaleFactor * 3.f, scaleFactor * 3.2f),
          onExploded,
          nullptr)};
  seq->setTag(static_cast<int>(Tag::explodin));
  CC::Animate *exp{
    CC::Animate::create(
          CC::AnimationCache::getInstance()->getAnimation(
            "AtomicBombExplotionAnimation"))};
  this->runAction(exp);
  this->set_is_exploding(true);
  // FIXME(denis): ver el tiempo de la animacion con el de la secuencia
  this->runAction(seq);
  weapon_discharged_callback_();
}
