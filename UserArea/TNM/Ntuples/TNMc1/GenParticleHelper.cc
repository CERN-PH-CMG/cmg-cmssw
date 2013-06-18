//-----------------------------------------------------------------------------
// Package:     PhysicsTools
// Sub-Package: TheNtupleMaker
// Description: Add user-defined methods
// Created:     Tue Jan 19, 2010 HBP
// Updated:     Mon Mar 08, 2010 Sezen & HBP - add triggerBits class
//              Tue Aug 24, 2010 HBP - add HcalNoiseRBXHelper
//              Thu Sep 02, 2010 HBP - update to new version of HelperFor
//                               HBP - move classes to separate files
//$Revision: 1.2 $
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "FWCore/Framework/interface/Event.h"
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/GenParticleHelper.h"
//-----------------------------------------------------------------------------
using namespace edm;
using namespace reco;
using namespace std;
//-----------------------------------------------------------------------------
// This is called once per job
// Important: remember to initialize base class
GenParticleHelper::GenParticleHelper() : HelperFor<reco::GenParticle>() {}

// Called once per event
void
GenParticleHelper::analyzeEvent()
{
  // Initialize string representation/position map
  if ( event == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
                         "event pointer is ZERO");
  
  // Get genparticles:
  edm::Handle<GenParticleCollection> handle;
  // For now, hard-code getByLabel
  event->getByLabel("genParticles", handle);
  if (!handle.isValid())
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
                         "GenParticle handle is invalid");

  // Write a unique string for each genparticle
  // keeping only particles with status=3
  // Note: these occur first in particle list

  amap.clear();
  
  for(unsigned int i = 0; i < handle->size(); i++) 
    {
      const GenParticle* p = &((*handle)[i]);

      //if ( p->status() != 3 ) break;

      bool store = false;
      //store particles with status 3
      if(p->status()==3)
        store = true;
      //store the decaying b and c hadrons 
      //(allways status 2)
      else if(p->status()==2)
        {
          int p_h = abs(p->pdgId()%1000/100);
          int p_t = abs(p->pdgId()%10000/1000);
          int taste = -1;
          //if the particle is a b meson or baryon
          if(p_h==5 || p_t == 5)
            taste = 5;
          //if the particle is a c meson or baryon
          else if(p_h==4 || p_t == 4)
            taste = 4;
          if(taste >0)
            {
              //store the particle
              store = true;
              //if it decays to a particle of different 'taste'
              for(unsigned int d = 0;d<p->numberOfDaughters();++d)
                {
                  int d_h = abs(p->daughter(d)->pdgId()%1000/100);
                  int d_t = abs(p->daughter(d)->pdgId()%10000/1000);
                  if(d_h==taste||d_t==taste)
                    store = false;
                }
            }
        }
      else if(p->status()==1)
        {
          if(p->pt() > 2)
            store = ( abs(p->pdgId()) == 11 || abs(p->pdgId()) == 13 || abs(p->pdgId())== 15);
        }
      else if((p->numberOfMothers()>0)&&(p->mother()->pdgId()>=23)&&(p->mother()->pdgId()<=25))
        store=true;
      if ( !store ) break;

      char particle[255];
      sprintf(particle,"%d%d%f%f%f%f",
              p->pdgId(), 
              p->status(), 
              p->px(), 
              p->py(), 
              p->pz(), 
              p->energy());
      amap[string(particle)] = i;
    }
}

void 
GenParticleHelper::analyzeObject()
{
  if ( object == 0 )
    throw edm::Exception(edm::errors::Configuration,
                         "\nGenParticleHelper - " 
                         "object pointer is ZERO");

  // save only status = 3 particles

  
  bool store = false;
  //store particles with status 3
  if(object->status()==3)
    store = true;
  //store the decaying b and c hadrons 
  //(allways status 2)
  else if(object->status()==2)
    {
      int p_h = abs(object->pdgId()%1000/100);
      int p_t = abs(object->pdgId()%10000/1000);
      int taste = -1;
      //if the particle is a b meson or baryon
      if(p_h==5 || p_t == 5)
        taste = 5;
      //if the particle is a c meson or baryon
      else if(p_h==4 || p_t == 4)
        taste = 4;
      if(taste >0)
        {
          //store the particle
          store = true;
          //if it decays to a particle of different 'taste'
          for(unsigned int d = 0;d<object->numberOfDaughters();++d)
            {
              int d_h = abs(object->daughter(d)->pdgId()%1000/100);
              int d_t = abs(object->daughter(d)->pdgId()%10000/1000);
              if(d_h==taste||d_t==taste)
                store = false;
            }
        }
    }
  else if(object->status()==1)
    {
      if(object->pt() > 2)
        store = ( abs(object->pdgId()) == 11 || abs(object->pdgId()) == 13 || abs(object->pdgId())== 15);
    }

  if ( !store )
  //  if ( object->status() != 3 )
    {
      count = 0;
      return;
    }
  
  // Find the ordinal value of first and last mothers by comparing the 
  // string representation of mothers with the string representation of 
  // each gen-particle in the list:

  char particle[255];

  mothers_.clear();
  for(unsigned int j=0; j < object->numberOfMothers(); j++) 
    {
      const GenParticle* m = 
        dynamic_cast<const GenParticle*>(object->mother(j));
      if ( m == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f",
              m->pdgId(), 
              m->status(), 
              m->px(), 
              m->py(), 
              m->pz(), 
              m->energy());
      if ( amap.find(string(particle)) != amap.end() ) 
        mothers_.push_back( amap[string(particle)] );
    }

  // Find the ordinal value of first and last daughters by comparing the 
  // string representation of daughters with the string representation of 
  // each gen-particle in the list:

  daughters_.clear();
  for(unsigned int j=0; j < object->numberOfDaughters(); j++) 
    {
      const GenParticle* d = 
        dynamic_cast<const GenParticle*>(object->daughter(j));
      if ( d == 0 ) continue;
      sprintf(particle,"%d%d%f%f%f%f", 
              d->pdgId(),  
              d->status(), 
              d->px(), 
              d->py(), 
              d->pz(), 
              d->energy());
      if ( amap.find(string(particle)) != amap.end() ) 
        daughters_.push_back( amap[string(particle)] );
    }
}

GenParticleHelper::~GenParticleHelper() {}

int      GenParticleHelper::charge() const { return object->charge(); }
int      GenParticleHelper::pdgId()  const { return object->pdgId(); }
int      GenParticleHelper::status() const { return object->status(); }
double   GenParticleHelper::energy() const { return object->energy(); }
double   GenParticleHelper::pt()     const { return object->pt(); }
double   GenParticleHelper::eta()    const { return object->eta(); }
double   GenParticleHelper::phi()    const { return object->phi(); }
double   GenParticleHelper::mass()   const { return object->mass(); }

int 
GenParticleHelper::firstMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.front();
  else
    return -1;
}

int 
GenParticleHelper::lastMother() const
{
  if ( mothers_.size() > 0 )
    return mothers_.back();
  else
    return -1;
}

int 
GenParticleHelper::firstDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.front();
  else
    return -1;
}

int 
GenParticleHelper::lastDaughter() const
{
  if ( daughters_.size() > 0 )
    return daughters_.back();
  else
    return -1;
}

//-----------------------------------------------------------------------------
// Synonym:
//-----------------------------------------------------------------------------
GParticle::GParticle() : GenParticleHelper() {}
GParticle::~GParticle() {}

