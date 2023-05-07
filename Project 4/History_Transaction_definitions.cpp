#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
//constructor definition outside of class you have to use class name and :: 
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date, unsigned int month_date,  unsigned year_date, bool buy_sell_trans,  unsigned int number_shares, double trans_amount ){
  //set private variable of the class so that they are accecible in order to be able to read or modify the value
  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  shares = number_shares; 
  amount = trans_amount;

  trans_id = assigned_trans_id;
  assigned_trans_id ++;

  if(buy_sell_trans == true){
    trans_type = "Buy";
  }
  else{
    trans_type = "Sell";
  }

  //for histroy functions 
  p_next = nullptr;

  acb = 0;
  acb_per_share = 0;
  share_balance = 0;
  cgl = 0;
}

// Destructor
// TASK 1
//leave blank
Transaction::~Transaction(){
}

//


// Overloaded < operator.
// TASK 2
bool Transaction::operator<( Transaction const &other ){
  if(this->year < other.year){ //year less than other
    return true;
  }

  else if(this->year == other.year){ //year and other are equal
    if(this->month < other.month){ //if month is less than other month
      return true;
    }

    else if(this->month == other.month){//month is equal to other month
      if(this->day < other.day){//if day is less than other day
        return true;
      }

      else if(this->day == other.day){ //if day is equal to other day
        if(this->trans_id >= other.trans_id){ //if trans id is greater than other trans id
          return true;
        }else{
          return false;
        }
      }

      else{ //if day is greater than other day
        return false;
      }
    }

    else{ //month is greater than other
      return false;
    }
  }  

  else{ //if year is greater than other
    return false;
  }
}

// GIVEN
// Member functions to get values.
//
//const means the address should never change
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
History::History(){
  p_head = nullptr;
}
//


// Destructor
// TASK 3
//use while loop until get next isnt a nullptr
History::~History(){
  Transaction *temp = nullptr;
  while(p_head != nullptr){
    temp = p_head;
    p_head = p_head -> get_next();
    delete temp;
  }
  temp = nullptr;
}
//

// read_history(...): Read the transaction history from file.
// TASK 4
/*create a linked list with the info from the txt file 
make a temp transaction
open the file 
call insert on temp 
close file after all lines have been read*/
void History::read_history(){
  ece150::open_file();
  while(ece150::next_trans_entry()) {
    Transaction *temp{new Transaction ( ece150::get_trans_symbol(), ece150::get_trans_day(), 
    ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount())};
  
    insert(temp);
  }
  ece150::close_file();
}
//

// insert(...): Insert transaction into linked list.
//use a while loop
//if smth is null break the while loop because that is the end of the list 
//
void History::insert(Transaction *p_new_trans){
  //p_head is the linked list
  if(p_head == nullptr){
    p_head = p_new_trans;
  }
  else{
    Transaction *temp= p_head;
    while(temp->get_next() != nullptr){
      temp = temp->get_next();
    }
    temp->set_next(p_new_trans);
  }
}
// TASK 5
//


// sort_by_date(): Sort the linked list by trade date.
/*Sorts the linked list in ascending order of trade date (or transaction 
id). Uses the overloaded less-than operator to define sort order*/
void History::sort_by_date(){
  //sort with insert sort
  Transaction *temp = p_head; //temporary linked list
  Transaction *current = p_head->get_next(); //current node to be compared with previous
  Transaction *previous = p_head; //previous node to be compared with current

  if (current == nullptr) {
    return;
  }
  while (current != nullptr) { //go through whole list - last node points to nullptr

    if(*previous < *current){  //if current is larger than previous (e.g 4, 5) those two are already sorted so go to the next ones
      current = current->get_next();
      previous = previous->get_next();
    }
    else{ //previous is larger than current (e.g 5, 4)

      if(*current < *p_head){ //switch p_head node and set new previous and current for next comparison
        previous->set_next(current->get_next());
        current->set_next(p_head);
        p_head = current;
      }
      else{
        temp = p_head; //temporary list

        while ((temp->get_next() != nullptr) && (*temp->get_next() < *current)) {
          temp = temp->get_next(); //go through list to get the node previous of current
        }

        previous->set_next(current->get_next()); //set next previous and current for next comparison
        current->set_next(temp->get_next());
        temp->set_next(current); //set next temp to set the next current 
      }
    }
    current = previous->get_next(); //update current
  }
}
// TASK 6
//


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
/*Walks through the linked-list and updates the following private member variables: acb, acb_per_share,
share_balance, and cgl for each Transaction. See section on Computing gains and losses using Adjusted Cost
Base for additional details on how these values are calculated*/
/*
acb - add the total amount paid for purchases 
share_balance - add the share balances 
acb_per_share - divide acb by the share balance
cgl - subtract # of shares sold * acb/share
*/
void History::update_acb_cgl(){
  Transaction *temp = p_head; //temp list to manipulate
  //variables to hold temp values
  double prevamount{}; //holds previous acb
  double prevshare{}; //holds previous share balance 
  double prev_acb_per_share{}; //holds previous acb/share for acb/share during sell
  double acb_per_share{}; //holds different acb/share for calculating acb during sell

  while(temp != nullptr){ //go through whole list - last node points to nullptr 

    //calculate acb
    if(temp->get_trans_type() == true){ //buy
      temp->set_acb(prevamount + temp->get_amount());
    }
    else{//sell
      temp->set_acb(prevamount - (acb_per_share*temp->get_shares()));
    }

    //calculate share balance 
    if(temp->get_trans_type() == true){ //buy
      temp->set_share_balance(prevshare + temp->get_shares());
    }
    else{//sell
      temp->set_share_balance(prevshare - temp->get_shares());
    }

    //calculate acb per share 
    if(temp->get_trans_type() == true){ //buy 
      temp->set_acb_per_share(temp->get_acb()/temp->get_share_balance());
      prev_acb_per_share = temp->get_acb_per_share();
    }
    else{//sell
      temp->set_acb_per_share(prev_acb_per_share);//doesnt change take previous per share value
    }
    
    //calculate cgl
    if(temp->get_trans_type() == false){//sell
      temp->set_cgl(temp->get_amount() - (temp->get_shares()*prev_acb_per_share));
    }

    //reset temp variables for next line
    prevamount = temp->get_acb();
    acb_per_share = temp->get_acb_per_share();
    prevshare = temp->get_share_balance();
    temp = temp->get_next();
  }
}
//


// compute_cgl(): )Compute the ACB, and CGL.
/*Computes the capital gains or capital losses for every transaction in the history of transactions and updates
the respective instances in the linked list. In addition, this function returns the total capital gains for the
specified year. See section on Computing gains and losses using Adjusted Cost Base for additional details on
how to the CGL is calculated*/
double History::compute_cgl(unsigned int year){
  Transaction *temp = p_head;
  double cgltotal{};

  while(temp != nullptr){
    if(year == temp->get_year()){ //filter through all the cgls in a specified year 
      cgltotal += temp->get_cgl(); //adds all of them 
    }
    temp = temp->get_next(); //go through all the nodes 
  }

  return cgltotal;
}
// TASK 8


// print() Print the transaction history.
//TASK 9
void History::print(){
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  Transaction *temp{};
  temp = p_head;

  while(temp != nullptr){
    temp->print();
    temp = temp-> get_next();
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}
//

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
//g++ -o main .\History_Transaction_definitions.cpp .\project4.cpp -std=c++11